<?php
/*
 * newsdata.io parser for FujiNet
 *
 * This is the viewer that sends news articles to FujiNet.
 *
 * General variables:
 *   t: translation mode
 *      n=none (default)
 *      a=ATASCII
 *      cr=carriage return
 *      lf=line feed
 *      crlf=carriage return & line feed
 *   ps: page size, ColumnsxRows
 *      (default Atari 39x22)
 *   p: page number to display (default 1)
 *
 * Category variables:
 *   c: category name:
 *   	top, world, science, business, technology,
 *		health, entertainment, politics, sports
 *   l: limit results to this many (default 10)
 *
 * Article variables:
 *   a: article ID from search results
 *
 * If category and article ID are both provided, article
 * will be returned and category ignored.
 */
require_once('vendor/autoload.php');
require_once('newsfunc.php');

use html2text\html2text;

if ( isset($_GET['t']) ) // Translation Mode
	$transMode = $_GET['t'];
else
	$transMode = "n"; // Default none

if ( isset($_GET['l']) ) // Results Limit
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

/* If we have an article, load it. If not, check
 * for category and define default if not given
 */
if ( isset($_GET['a']) )
{
	$articleID = $_GET['a'];
	$category = NULL;
}
elseif ( isset($_GET['c']) )
{
	// Make sure category provided is valid
	$category = NULL;
	foreach($categoryArray as $thisCat)
	{
		if($thisCat == $_GET['c'])
		{
			$category = $_GET['c'];
			break;
		}
	}
	if ($category == NULL)
	{
		echo translate_text("ERROR: invalid category: ".$_GET['c']).line_ending();
		exit(1);
	}
	$articleID = NULL;
}
else
{
	$category = "top"; // Default
	$articleID = NULL;
}

/* Database user / pass */
$dbuser = "dbusername";
$dbpass = 'dbpassword';
$dbname = "fujinews";

// Create Database connection
$dbconn = new mysqli("localhost", $dbuser, $dbpass, $dbname);

// Check Database connection
if ($dbconn->connect_error) {
  die("Connection failed: " . $dbconn->connect_error);
}

// Get and return an article
if ($articleID != NULL)
{
	$articleSQL = "SELECT * FROM `articles` WHERE `id` = '".$articleID."'";
	$result = mysqli_query($dbconn, $articleSQL);
	if(mysqli_num_rows($result) > 0)
	{
		$article = mysqli_fetch_array($result);
		$articleText = wordwrap(translate_text(stripGarbage($article['content'])), $col, line_ending(), true); // Wrap text to columns
		
		$separator = line_ending();
		$line = strtok($articleText, $separator);
		$output = NULL;
		$linecount = 1;
		$pagecount = 1;
		
		while ($line !== false) {
			while($linecount <= $row){
				if($pagecount == $page)
					$output .= $line . line_ending();
		
				$line = strtok( $separator ); // do it again
				$linecount++;
			}
			$pagecount++;
			$linecount = 1; // reset line counter
		}

	}
	else
	{
		echo translate_text("ERROR: Article not found").line_ending();
		exit(1);
	}
	$pagecount--; // We've gone too far, back it up
	if ($output == NULL) // Something went wrong, try to find out and error
	{
		if ($page > $pagecount)
		{
			echo translate_text("ERROR: page ".$page." of ".$pagecount." out of range").line_ending();
			exit(1);
		}
		else
		{
			echo translate_text("ERROR: no article data to display").line_ending();
			exit(1);
		}
	}
	echo translate_text($article['title']).line_ending();
	echo translate_text($article['pubdate']).line_ending();
	echo translate_text($article['source']).line_ending();
	echo $page."/".$pagecount.line_ending();
	echo $output;
	exit(0);
}

// Get and return list of articles in a category
$categorySQL = "SELECT COUNT(*) FROM `articles` WHERE `category` = '".$category."';";
if(!$catResult = mysqli_query($dbconn, $categorySQL))
	echo "ERROR: " . $addSQL . "<br>" . $dbconn->error . "<br>\n";
$resultArray = mysqli_fetch_array($catResult);
$pageTotal = ceil($resultArray[0]/$limit);
if ($page > $pageTotal || $page < 1)
{
	echo translate_text("ERROR: page ".$page." of ".$pageTotal." out of range").line_ending();
	exit(1);
}
echo $page."/".$pageTotal.line_ending();
$articleSQL = "SELECT `id`, `pubdate`, `title` FROM `articles` WHERE `category` = '".$category."' ORDER BY `pubdate` DESC LIMIT ".($limit*$page);
$result = mysqli_query($dbconn, $articleSQL);
if(mysqli_num_rows($result) > 0)
{
	$limitcount = 1;
	$pagecount = 1;
	while($thisArticle = mysqli_fetch_array($result))
	{
		if ($limitcount <= $limit && $pagecount == $page)
		{
			echo translate_text($thisArticle['id']."|".$thisArticle['pubdate']."|".convert_ascii($thisArticle['title'])).line_ending();
		}
		if ($limitcount == $limit)
		{
			$limitcount = 1; // Reset the counter
			$pagecount++; // Next page
		}
		else
			$limitcount++;
	}
}
else
{
	echo translate_text("ERROR: '".$category."' category empty").line_ending();
}
?>
