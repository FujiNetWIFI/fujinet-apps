#!/usr/bin/php
<?php
/* Skip header; assuming:
P6
160 80
255
*/

for ($i = 0; $i < 3; $i++) {
  fgets(STDIN);
}

/* Generate array to store image, so we can go over it a few times */
$px = array();
for ($y = 0; $y < 80; $y++) {
  $px[$y] = array();
  for ($x = 0; $x < 160; $x++) {
    $px[$y][$x] = array(0, 0, 0);
  }
}

/* Space to store colors: */

$colors = array();

/* Load the image */
for ($y = 0; $y < 80; $y++) {
  for ($x = 0; $x < 160; $x++) {
    $r = ord(fgetc(STDIN));
    $g = ord(fgetc(STDIN));
    $b = ord(fgetc(STDIN));
    $px[$y][$x][0] = $r;
    $px[$y][$x][1] = $g;
    $px[$y][$x][2] = $b;

    $c = sprintf("%02x%02x%02x", clamp($r), clamp($g), clamp($b));

    if (!in_array($c, $colors)) {
      fprintf(STDERR, "Adding color $c\n");
      $colors[] = $c;
    }
  }
}

$palette = array();
$idx = 0;
foreach ($colors as $c) {
  $palette[$c] = $idx++;
}

$b = array(0, 0, 0, 0);

for ($y = 0; $y < 80; $y++) {
  for ($x = 0; $x < 160; $x += 4) {
    for ($i = 0; $i < 4; $i++) {
      $color = sprintf("%02x%02x%02x",
        clamp($px[$y][$x + $i][0]),
        clamp($px[$y][$x + $i][1]),
        clamp($px[$y][$x + $i][2])
      );

      $b[$i] = $palette[$color];
    }

    $c = chr(($b[0] * 64) + ($b[1] * 16) + ($b[2] * 4) + $b[3]);
    fwrite(STDOUT, $c, 1);
  }
}

/* Determine the Atari colors utilize, so we can send bytes for the four
   color palette entries */

$fi = fopen("atari128.ppm", "r");
/* Skip header; assuming:
P6
8 16
255
*/

for ($i = 0; $i < 3; $i++) {
  fgets($fi);
}

$atari_colors = array();
for ($i = 0; $i < 128; $i++) {
  $r = ord(fgetc($fi));
  $g = ord(fgetc($fi));
  $b = ord(fgetc($fi));

  $c = sprintf("%02x%02x%02x", clamp($r), clamp($g), clamp($b));
  $atari_colors[$c] = ($i * 2);
}
fclose($fi);

foreach ($palette as $rgb => $_) {
  if (!array_key_exists($rgb, $atari_colors)) {
    fprintf(STDERR, "color $rgb doesn't exist\n");
    fwrite(STDOUT, chr(0), 1);
  } else {
    $c = chr($atari_colors[$rgb]);
    fprintf(STDERR, "Atari color $rgb = %s\n", $atari_colors[$rgb]);
    fwrite(STDOUT, $c, 1);
  }
}

/* FIXME: This function shouldn't be necessary...? */
function clamp($x) {
  return($x);
 // return (floor($x / 16) * 16);
}

