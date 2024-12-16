<?php
/*
 * Functions for fujinews api
 */
require_once('vendor/autoload.php');

// Categories selected for news articles
$categoryArray = array
	(
		0 => "top",
		1 => "world",
		2 => "science",
		3 => "business",
		4 => "technology",
		5 => "health",
		6 => "entertainment",
		7 => "politics",
		8 => "sports"
	);

// Remove any junk we don't want from the text
function stripGarbage($str)
{
	// Washington Post articles start with this
	$str = preg_replace("#Placeholder while article actions load#", "", $str);
	
	// Convert emojis to text tags inside colons
	$str = Emoji\replace_emoji($str, ':', ':');
	
	// Convert text to ASCII
	$str = convert_ascii($str);
	
	return $str;
}

// Returns line ending for selected translation
function line_ending()
{
	global $transMode;

	switch($transMode)
	{
		case "a": // ATASCII
		case "ant": // ANTIC Internal
			return chr(155);
			break;
		case "br": // HTML break
			return "<br>\n";
			break;
		case "cr": // Carriage Return
			return "\r";
			break;
		case "lf": // Line Feed
			return "\n";
			break;
		case "crlf": // Carriage Return & Line Feed
			return "\r\n";
			break;
		case "n":
		default: // Do Nothing
			return;
			break;
	}
}

// Convert text to ATASCII
function convert_atascii($str)
{
	$newtext = "";
	for ( $pos=0; $pos < strlen($str); $pos ++ )
	{
		$byte = substr($str, $pos);
		if(ord($byte) == 10) // Replace $0A with SPACE
			$newtext .= chr(32);
		else
			$newtext .= chr(ord($byte));
	}
	return $newtext;
}

// Convert text to ANTIC Internal Map
function convert_antic($str)
{
	// Convert to ATASCII First
	$str = convert_atascii($str);

	$len = strlen($str);
	for($j=0; $j<$len; $j++)
	{
		$intval = ord($str[$j]);
		if ($intval < 32)
		{
			$str[$j] = chr($intval+64);
		}
		elseif ($intval < 96)
		{
			$str[$j] = chr($intval-32);
		}
		// else the numbers are the same, move along
	}
	return $str;
}

// Returns string translated
function translate_text($str)
{
	global $transMode;

	switch($transMode)
	{
		case "a": // ATASCII
			return convert_atascii($str);
			break;
		case "ant": // ANTIC Internal
			return convert_antic($str);
			break;
		case "br": // HTML break
			return $str;
			break;
		case "cr": // Carriage Return
			return $str;
			break;
		case "lf": // Line Feed
			return $str;
			break;
		case "crlf": // Carriage Return & Line Feed
			return $str;
			break;
		case "n":
		default: // Do Nothing
			return $str;
			break;
	}
}

/**
 * Remove any non-ASCII characters and convert known non-ASCII characters
 * to their ASCII equivalents, if possible.
 *
 * @param string $string
 * @return string $string
 * @author Jay Williams <myd3.com>
 * @license MIT License
 * @link http://gist.github.com/119517
 */
function convert_ascii($string)
{
    // Replace Single Curly Quotes
    $search[]  = chr(226) . chr(128) . chr(152);
    $replace[] = "'";
    $search[]  = chr(226) . chr(128) . chr(153);
    $replace[] = "'";
 
    // Replace Smart Double Curly Quotes
    $search[]  = chr(226) . chr(128) . chr(156);
    $replace[] = '"';
    $search[]  = chr(226) . chr(128) . chr(157);
    $replace[] = '"';
 
    // Replace En Dash
    $search[]  = chr(226) . chr(128) . chr(147);
    $replace[] = '--';
 
    // Replace Em Dash
    $search[]  = chr(226) . chr(128) . chr(148);
    $replace[] = '---';
 
    // Replace Bullet
    $search[]  = chr(226) . chr(128) . chr(162);
    $replace[] = '*';
 
    // Replace Middle Dot
    $search[]  = chr(194) . chr(183);
    $replace[] = '*';
 
    // Replace Ellipsis with three consecutive dots
    $search[]  = chr(226) . chr(128) . chr(166);
    $replace[] = '...';
 
    // Apply Replacements
    $string    = str_replace($search, $replace, $string);
 
    // Remove any non-ASCII Characters
    $string    = preg_replace("/[^\x01-\x7F]/", "", $string);
 
    return $string;
}
?>
