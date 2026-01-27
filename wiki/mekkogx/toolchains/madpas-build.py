#!/usr/bin/env python3
import argparse
import tempfile
import subprocess
import os
import shlex

def build_argparser():
  parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument("ofile", nargs="+", help="pseudo object file")
  parser.add_argument("-o", "--output", help="save output into")
  parser.add_argument("-L", "--unit-path", action="append",
                      help="Add directory to the Pascal unit include path")
  parser.add_argument("-I", "--include-path", action="append",
                      help="Add directory to the mads assembler include path")
  parser.add_argument("--rc", help="rc file")
  parser.add_argument("--flag", action="store_true", help="flag to do something")
  return parser

def is_unit_file(path):
  with open(path, 'r') as f:
    for line in f:
      if line.strip().lower().startswith("unit "):
        return True
  return False

def main():
  args = build_argparser().parse_args()

  pascal_files = []
  assembly_files = []
  for path in args.ofile:
    with open(path, "r") as file:
      component = file.readline().strip().split(maxsplit=2)
      if component[1] == "rcpascal":
        pascal_files.append(component[2])
        continue

      assembly_files.append(component)

  if len(pascal_files) != 1:
    # Remove unit files from the list and make sure that they have an
    # uppercase bas and lowercase extension because Mad-Pascal changes
    # the case of filenames.
    unit_files = [x for x in pascal_files if is_unit_file(x)]
    for unit in unit_files:
      unit_dir = os.path.dirname(unit)
      unit_base, unit_ext = os.path.splitext(os.path.basename(unit))
      unit_base_uc = unit_base.upper()
      unit_uc = os.path.join(unit_dir, unit_base_uc + unit_ext)
      if not os.path.exists(unit_uc):
        os.symlink(unit_base + unit_ext, unit_uc)
    pascal_files = list(set(pascal_files) - set(unit_files))
    if len(pascal_files) != 1:
      print("Wrong amount of Pascal files", pascal_files)
      exit(1)

  pas_path = pascal_files[0]

  workdir = os.path.abspath(os.path.dirname(pas_path))
  if assembly_files:
    tfile = tempfile.NamedTemporaryFile(mode="w+t", suffix=".rc", dir=workdir,
                                        delete_on_close=False)
    for asm in assembly_files:
      asm_path = os.path.abspath(asm[2])
      asm_rel = os.path.relpath(asm_path, workdir)
      print(" ".join([asm[0], asm[1], asm_rel]), file=tfile)

    if args.rc:
      raise NotImplementedError

    tfile.close()

    ptemp = tempfile.NamedTemporaryFile(mode="w+t", suffix=".pas", dir=workdir,
                                        delete_on_close=False)
    with open(pas_path, "r+t") as pfile:
      for line in pfile:
        ptemp.write(line)
    #print(f"{{$r {os.path.basename(tfile.name)}}}", file=ptemp)
    ptemp.close()

    pas_path = ptemp.name

  cmd = ["mp", os.path.relpath(pas_path, workdir)]
  for unit_path in args.unit_path:
    cmd.append(f"-ipath:{unit_path}")
  atemp = tempfile.NamedTemporaryFile(suffix=".asm", dir=workdir,
                                      delete_on_close=False)
  cmd.append(f"-o:{os.path.relpath(atemp.name, workdir)}")
  print(" ".join([shlex.quote(x) for x in cmd]), flush=True)
  try:
    # FIXME - filter output from command to rewrite error lines so they name the input file
    subprocess.run(cmd, cwd=workdir, check=True, stderr=subprocess.STDOUT)
  except:
    print("Failed to compile")
    exit(1)

  del tfile, ptemp

  cmd = ["mads", atemp.name, "-x"]
  for include_path in args.include_path:
    cmd.append(f"-i:{include_path}")
  if args.output:
    output_path = os.path.abspath(args.output)
    cmd.append(f"-o:{output_path}")
  print(" ".join([shlex.quote(x) for x in cmd]), flush=True)
  subprocess.run(cmd, cwd=workdir, check=True, stderr=subprocess.STDOUT)

  return

if __name__ == "__main__":
  exit(main() or 0)
