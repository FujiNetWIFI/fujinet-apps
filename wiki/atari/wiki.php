<?php
/*
 * Wikipedia API for FujiNet
 *
 * Generic variables:
 *   t: translation mode
 *      n=none (default)
 *      a=ATASCII
 *      cr=carriage return
 *      lf=line feed
 *      crlf=carriage return & line feed
 *
 * Search variables:
 *   s: search term
 *   l: limit search to this many (default 10)
 *
 * Article variables:
 *   a: article pageid from search results
 *   p: page number to display (default 1)
 *   ps: page size, ColumnsxRows
 *      Default: Atari 39x22
 *
 */

// Variables
$apiURL = "https://en.wikipedia.org/w/api.php";
if ( isset($_GET['t']) ) // Translation Mode
	$transMode = $_GET['t'];
else
	$transMode = "n"; // Default none

if ( isset($_GET['l']) ) // Search Limit
	$limit = $_GET['l'];
else
	$limit = "10"; // Default 10

if ( isset($_GET['p']) ) // Page Number
	$page = $_GET['p'];
else
	$page = "1"; // Default 1

if ( isset($_GET['ps']) ) // Page Size
{
	$ps = explode("x", $_GET['ps']);
	$col = $ps['0'];
	$row = $ps['1'];
}
else
{
	$col = 39; // Default Atari 39
	$row = 22; // Default Atari 22
}

// Returns line ending for selected translation
function line_ending()
{
	global $transMode;

	switch($transMode)
	{
		case "a": // ATASCII
			return chr(155);
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

function bold($thisLine)
{
	global $transMode;

	switch($transMode)
	{
		case "a": // ATASCII
			$thisLine = preg_replace('/== /', "", $thisLine);
			$thisLine = preg_replace('/ ==/', "", $thisLine);
			$newLine = "";
			for ( $pos=0; $pos < strlen($thisLine); $pos ++ ) {
				$byte = substr($thisLine, $pos);
				$newLine .= chr(ord($byte)+128); // Inverse ATASCII
			}
			return $newLine;
			break;
		case "cr": // Carriage Return
		case "lf": // Line Feed
		case "crlf": // Carriage Return & Line Feed
		case "n":
		default: // Do Nothing
			return $thisLine;
			break;
	}
}

// Wiki Search
if ( isset($_GET['s']) )
{
	$searchTerm = preg_replace("# #", "_", urldecode($_GET['s'])); // convert the encoded spaces if any
	$searchURL = $apiURL."?action=query&list=search&format=json&srlimit=".$limit."&srsearch=".$searchTerm;
	$searchJSON = json_decode(file_get_contents($searchURL), true);
	foreach( $searchJSON['query']['search'] as $thisResult )
	{
		// 'pageid' is the article number, 'title' is the title
		echo $thisResult['title'].", ".$thisResult['pageid'].line_ending();
	}

	exit(); // Search complete
}

// Wiki Article
if ( isset($_GET['a']) )
{
	$articleURL = $apiURL."?action=query&explaintext=true&prop=extracts&format=json&pageids=".$_GET['a'];
	$articleJSON = json_decode(file_get_contents($articleURL), true);

	foreach ($articleJSON['query']['pages'] as $thisArticle)
	{
		$articleHTML = $thisArticle['extract'];
		$articleTitle = $thisArticle['title'];
		$articleText = wordwrap($thisArticle['extract'], $col, "\n"); // Wrap text to columns
	}

	// Go line by line and parse the text into ATASCII
	$separator = "\n";
	$line = strtok($articleText, $separator);
	$output = "";
	$linecount = 1;
	$pagecount = 1;
	
	while ($line !== false) {
		while($linecount <= $row){
			// Check if this is a header line and convert
			if ( preg_match("/== /", substr($line, 0, 6)) ) // line starts with ==
			{
				$line = bold($line);
			}
			
			if($pagecount == $page)
				$output .= $line . line_ending();
	
			$line = strtok( $separator ); // do it again
			$linecount++;
		}
		$pagecount++;
		$linecount = 1; // reset line counter
	}
	
	// Make sure headline fits # of columns
	$articleTitle = substr($articleTitle, 0, ($col - 8));

	// Bold the Title
	$headline = bold($articleTitle);
	
	echo $headline . " Pg:".$page."/" . ($pagecount - 1) . line_ending() . $output;
	
	exit(); // Article Display Complete
}

echo "ERROR: No article or search query";
?>