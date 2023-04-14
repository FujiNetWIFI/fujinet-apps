import os

def read_file(file):
    full_file = os.getcwd() + "/" + file
    
    print(f"Reading file {full_file}...")
    with open(full_file) as f:
        contents = f.readlines()
    
    return contents

def write_file(file, contents):
    full_file = os.getcwd() + "/" + file
    
    print(f"Writing file {full_file}...")
    with open(full_file, "w") as f:
        contents = f.writelines(contents)
    
    return contents

def convert_map_mag_line_to_c(mag_line, binary=False):
    c_line = ""
    tag = "MP:"
    data = mag_line.find(tag)
    mag_line = mag_line[data+len(tag):].strip()
        
    while len(mag_line)>0:
        end_data = mag_line.find("|")
        if end_data > 0:
            data = mag_line[:end_data]
        else:
            data = mag_line
            
        value = int(data,10)
        
        c_line = c_line + format(value, '#04x') +","

        if end_data >= 0:
            mag_line = mag_line[end_data+1:]
        else:
            mag_line = ""

    return c_line


def convert_char_mag_line_to_c(mag_line, binary=True):
    c_line = ""
    mag_line = mag_line[3:].strip()
    while len(mag_line)>0:
        hex_str = "0x"+mag_line[:2]
        value = int(hex_str,16)
        
        if binary:
            c_line = c_line + format(value, '#010b')
            c_line = c_line + ",\n\t"
        else:
            c_line = c_line + format(value, '#04x')
            c_line = c_line + ","

        mag_line = mag_line[2:]
        
        
    return c_line
        
def convert_colour_mag_line_to_c(mag_line, binary=True):
    c_line = ""
    tag = "CO:"
    data = mag_line.find(tag)
    mag_line = mag_line[data+len(tag):].strip()

    hex_str = "0x"+mag_line[:2]
    value = int(hex_str,16)
    if binary:
        c_line = c_line + format(value, '#010b')
    else:
        c_line = c_line + format(value, '#04x')

    c_line = c_line + "," 
        
    return c_line


def convert_sprite_mag_line_to_c(mag_line, binary=True):
    c_line = ""
    mag_line = mag_line[3:].strip()
    while len(mag_line)>0:
        hex_str = "0x"+mag_line[:2]
        value = int(hex_str,16)
        
        if binary:
            c_line = c_line + format(value, '#010b')
            c_line = c_line + ",\n\t"
        else:
            c_line = c_line + format(value, '#04x')
            c_line = c_line + ","
            
        
        mag_line = mag_line[2:]
        
    if binary:
        c_line = c_line[:-2]
        
        
    return c_line

def replace_c_with_mag_char(c_contents_in, c_start_replacement, mag_contents_in, c_mag_start_collection, binary_output=False):
    
    c_contents_out = [];
    
    # Find where our mag charset starts
    mag_linenum = 0
    for line in mag_contents_in:
        line_in = line.strip()
            
        if line_in.find(c_mag_start_collection)>=0:
            mag_linenum += 1
            break
        
        mag_linenum += 1
    
    
    # Find where the start replacing the charset
    c_linenum = 0
    char_set_size = 0
    start_replacing = False
    offset = 0;
    for line_in in c_contents_in:
            
        if line_in.find(c_start_replacement)>0:
            start_replacing = True
            break
        c_contents_out.append(line_in)
        c_linenum += 1
    
    if not start_replacing:
        print(f"Pattern '{c_mag_start_collection}' not found!")
        return
    else:
        
        while True:
            if mag_linenum >= len(mag_contents_in):
                break
            
            c_line = "\t\t" + convert_char_mag_line_to_c(mag_contents_in[mag_linenum].strip(), binary_output)
            comment = c_contents_in[c_linenum].find("//")
            if comment < 0:
                comment = " // line " + format(mag_linenum+1, '#4d') + " | char offset : "+ format(offset, '#4d') + " / " + format(offset, '#04x')
            else:
                comment = c_contents_in[c_linenum][comment:].strip()
            c_line = c_line + comment
            c_line = c_line + "\n"
            c_contents_out.append(c_line)
            char_set_size += 1
            if char_set_size > 255:
                break
            mag_linenum += 1
            offset      += 1
            c_linenum   += 1
            
      
        c_contents_out[len(c_contents_out)-1] = c_contents_out[len(c_contents_out)-1].replace(", //", "  //")
        
            
        while c_linenum < len(c_contents_in):
            c_contents_out.append(c_contents_in[c_linenum])
            c_linenum += 1
        
        
    return c_contents_out


def replace_c_with_mag_colour(c_contents_in, c_start_replacement, mag_contents_in, c_mag_start_collection, binary_output=False):
    
    c_contents_out = [];
    
    # Find where our mag charset starts
    mag_linenum = 0
    for line in mag_contents_in:
        line_in = line.strip()
            
        if line_in.find(c_mag_start_collection)>=0:
            mag_linenum += 1
            break
        
        mag_linenum += 1
    
    
    # Find where the start replacing the charset
    c_linenum = 0

    colour_set_size = 0
    start_replacing = False
    offset = 0;
    for line_in in c_contents_in:
            
        if line_in.find(c_start_replacement)>0:
            offset = 0
            start_replacing = True
            break
        else:
            c_contents_out.append(line_in)
            if colour_set_size > 0:
                colour_set_size += 1
            if colour_set_size >= 255:
                break
        
        if line_in.find(c_start_replacement) > 0:
            colour_set_size = 1
            
        c_linenum += 1
        offset    += 1
    
    if not start_replacing:
        print(f"Pattern '{c_mag_start_collection}' not found!")
        return
    else:
        
        while True:
            if mag_linenum >= len(mag_contents_in):
                break
            
            mag_line = mag_contents_in[mag_linenum].strip()

            c_line = "\t\t" + convert_colour_mag_line_to_c(mag_line, binary_output)
            c_line = c_line + " // line " + format(mag_linenum+1, '#4d') + " | colour offset : "+format(offset, '#4d')  + " / " + format(offset, '#04x')
            c_line = c_line + "\n"
            c_contents_out.append(c_line)
            colour_set_size += 1
            if colour_set_size > 255:
                break
            mag_linenum += 1
            offset      += 1
            
      
        c_contents_out[len(c_contents_out)-1] = c_contents_out[len(c_contents_out)-1].replace(", //", "  //")
        
        # append other data in file after };
        
        while c_linenum < len(c_contents_in):
            if c_contents_in[c_linenum].find("};") >= 0:
                break
            c_linenum += 1
        
        while c_linenum < len(c_contents_in):
            c_contents_out.append(c_contents_in[c_linenum])
            c_linenum += 1
        
        
    return c_contents_out


def replace_c_with_mag_map(c_contents_in, c_start_replacement, mag_contents_in, c_mag_start_collection, binary_output=False):
    
    c_contents_out = [];
    
    # Find where our mag map starts
    mag_linenum = 0
    for line in mag_contents_in:
        line_in = line.strip()
            
        if line_in.find(c_mag_start_collection)>=0:
            start_replacing = True
            mag_linenum += 4
            break
        mag_linenum += 1
        

    if not start_replacing:
        print(f"Pattern '{c_mag_start_collection}' not found!")
        return
    else:
        c_linenum = 0
        for line in c_contents_in:
            line_in = line.strip();
            c_contents_out.append(line_in + "\n")
            if line_in.find(c_start_replacement) >= 0:
                break

        
        while True:
            if mag_linenum >= len(mag_contents_in):
                break
            
            if mag_contents_in[mag_linenum][0] == "*":
                break
            
            mag_line = mag_contents_in[mag_linenum].strip()
            c_line = "\t" + convert_map_mag_line_to_c(mag_line, binary_output)
            comment = " // line " + format(mag_linenum+1, '#4d')
            c_line = c_line + "\n"
            c_contents_out.append(c_line)

            mag_linenum += 1
            c_linenum   += 1
            

        c_contents_out[len(c_contents_out)-1] = c_contents_out[len(c_contents_out)-1][:-2] + "\n"
        c_contents_out.append("};\n")
      
        
    return c_contents_out


def replace_c_with_mag_sprite(c_contents_in, c_start_replacement, mag_contents_in, c_mag_start_collection, binary_output=False):
    
    c_contents_out = [];
    
    # Find where our mag map starts
    mag_linenum = 0
    for line in mag_contents_in:
        line_in = line.strip()
            
        if line_in.find(c_mag_start_collection)>=0:
            start_replacing = True
            mag_linenum += 1
            break
        mag_linenum += 1
    

    if not start_replacing:
        print(f"Pattern '{c_mag_start_collection}' not found!")
        return
    else:
    
        c_linenum = 0
        for line in c_contents_in:
            line_in = line.strip();
            c_contents_out.append(line_in + "\n")
            if line_in.find(c_start_replacement) >= 0:
                break

        
        while True:
            if mag_linenum >= len(mag_contents_in):
                break
            
            if mag_contents_in[mag_linenum][0] == '*':
                break
            
            mag_line = mag_contents_in[mag_linenum].strip()
            c_line = "\t" + convert_sprite_mag_line_to_c(mag_line, binary_output)
            comment = " // line " + format(mag_linenum+1, '#4d')
            
            c_line = c_line + comment + "\n"
            c_contents_out.append(c_line)
            if mag_contents_in[mag_linenum][3:].strip() == '0000000000000000000000000000000000000000000000000000000000000000':
                break

            mag_linenum += 1
            c_linenum   += 1
            

        
        c_contents_out[len(c_contents_out)-1] = c_contents_out[len(c_contents_out)-1].replace(","+comment, " " + comment)
        c_contents_out.append("};\n")
      
        
    return c_contents_out


if __name__ == "__main__":
    
    mag_file_in     = "reversi.mag"
    
    c_charset_in    = "charset.c.txt"
    c_charset_out   = "charset.c"
    c_map_in        = "board.c"
    c_map_out       = "board.c"
    c_spriteset_in  = "spriteset.c.txt"
    c_spriteset_out = "spriteset.c"

    mag_contents = read_file(mag_file_in)
    print()
    c_contents   = read_file(c_charset_in)
        
    binary_output = False
    print("Adding character patterns...")
    c_contents = replace_c_with_mag_char(  c_contents, "//  space character", mag_contents, "* CHAR DEFS", binary_output)
    
    print("Adding character colours...")
    c_contents = replace_c_with_mag_colour(c_contents, "//  space color",    mag_contents,  "* CHAR COLORS")
    write_file(c_charset_out, c_contents)
    print()

    c_contents = read_file(c_map_in);
    print("Adding map...")
    c_contents = replace_c_with_mag_map(c_contents, "{", mag_contents, "* MAP #1")
    write_file(c_map_out, c_contents)
    print()

    c_contents = read_file(c_spriteset_in)
    print("Adding sprite patterns...")
    binary_output=True
    c_contents = replace_c_with_mag_sprite(c_contents, "{", mag_contents, "* SPRITES PATTERNS", binary_output)
    write_file(c_spriteset_out, c_contents)
    print()
     

    
