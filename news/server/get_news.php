<?php
/*
 * newsdata.io parser for FujiNet
 * https://newsdata.io/docs
 *
 * This script grabs the latest headlines from the categories,
 * then grabs the actual content and saves it in a database.
 * It can be run every hour for the 9 categories selected and
 * just hit the api limit of 200 requests per day.
 * 
 * ex request: https://newsdata.io/api/1/news?apikey=YOUR_API_KEY&category=top
 *
 * categories: business, entertainment, environment, food, health
 *  politics, science, sports, technology, top, world
 */
require_once('vendor/autoload.php');
require_once('newsfunc.php');

use fivefilters\Readability\Readability;
use fivefilters\Readability\Configuration;
use fivefilters\Readability\ParseException;
use html2text\html2text;

// API key to use for JSON requests
$api_key = "YOUR_API_KEY";
/* Countries Available
Argentina - ar, Australia - au, Austria - at, Belgium - be, Brazil - br
Bulgaria - bg, Canada - ca, China - cn, Colombia - co, Cuba - cu
Czech republic - cz, Egypt - eg, France - fr, Germany - de, Greece - gr
Hong kong - hk, Hungary - hu, India - in, Indonesia - id, Ireland - ie
Israel - il, Italy - it, Japan - jp, Latvia - lv, Lebanon - lb
Lithuania - lt, Malaysia - my, Mexico - mx, Morocco - ma, Netherland - nl
New zealand - nz, Nigeria - ng, North korea - kp, Norway - no, Pakistan - pk
Philippines - ph, Poland - pl, Portugal - pt, Romania - ro, Russia - ru
Saudi arabia - sa, Serbia - rs, Singapore - sg, Slovakia - sk, Slovenia - si
South africa - za, South korea - kr, Spain - es, Sweden - se, Switzerland - ch
Taiwan - tw, Thailand - th, Turkey - tr, Ukraine - ua, United arab emirates - ae
United kingdom - gb, United states of america - us, Venezuela - ve
*/
$country = "us";
/* Languages Available
 * Arabic - ar, Bosnian - bs, Bulgarian - bg, Chinese - zh
 * Croatian - hr, Czech - cs, Dutch - nl, English - en, French - fr
 * German - de, Greek - el, Hebrew - he, Hindi - hi, Hungarian - hu
 * Indonesian - in, Italian - it, Japanese - jp, Korean - ko, Latvian - lv
 * Lithuanian - lt, Malay - ms, Norwegian - no, Polish - pl, Portuguese - pt
 * Romanian - ro, Russian - ru, Serbian - sr, Slovak - sk, Slovenian - sl,
 * Spanish - es, Swedish - sv, Thai - th, Turkish - tr, Ukrainian - uk
 */
$lang = "en";
$baseURL = "https://newsdata.io/api/1/news?apikey=".$api_key."&country=".$country."&language=".$lang;

// Database Variables
$dbuser = "dbusername";
$dbpass = 'dbpassword';
$dbname = "fujinews";

// Create Database connection
$dbconn = new mysqli("localhost", $dbuser, $dbpass, $dbname);

// Check Database connection
if ($dbconn->connect_error) {
  die("Connection failed: " . $dbconn->connect_error);
}

$configuration = new Configuration();

foreach ($categoryArray as $thisCategory)
{
	$newsURL = $baseURL."&category=".$thisCategory;
	$newsJSON = json_decode(file_get_contents($newsURL), true);
	
	if($newsJSON['status'] == "success")
	{
		$contentCount = 0;
		foreach($newsJSON['results'] as $thisNews)
		{
			$configuration
				->setArticleByLine(false)
				->setFixRelativeURLs(true)
				->setOriginalURL($thisNews['link']);
			
			$readability = new Readability($configuration);
			
			$errFlag = 0; // Don't save the content if we get an error
	
			/* Skip sites that pull in garbage content or won't let you
			 * read the article
			 **/
			if( !preg_match("#yahoo.com#", $thisNews['link'])
			   && !preg_match("#usatoday.com#", $thisNews['link']))
			{
				// Check if we have the article already and skip if we do
				$checkSQL = "SELECT * FROM `articles` WHERE `link` LIKE '".$thisNews['link']."' AND `pubdate` = '".$thisNews['pubDate']."' AND `source` LIKE '".$thisNews['source_id']."'";
				$result = mysqli_query($dbconn, $checkSQL);
				if(mysqli_num_rows($result) == 0)
				{
					if(!$article_html = file_get_contents($thisNews['link']))
						$errFlag = 1;
			
					try
					{
						$readability->parse($article_html);
						$readable_article = strip_tags($readability->getContent(), '<a><ol><ul><li><br><p><small><font><b><strong><i><em><blockquote><h1><h2><h3><h4><h5><h6>');
						$readable_article = str_replace( 'strong>', 'b>', $readable_article ); //change <strong> to <b>
						$readable_article = str_replace( 'em>', 'i>', $readable_article ); //change <em> to <i>
						$readable_article = preg_replace('#<a.*?>(.*?)</a>#is', '\1', $readable_article); // Get rid of links
						
					}
					catch (ParseException $e)
					{
						$errFlag = 1;
					}

					if (!$errFlag)
					{
						$articleTitle = mysqli_real_escape_string($dbconn,stripGarbage($thisNews['title']));
						$plainText = new \Html2Text\Html2Text($readable_article);
						$output = mysqli_real_escape_string($dbconn, stripGarbage($plainText->getText()));
						$addSQL  = "INSERT INTO `articles` (`id`, `title`, `link`, `pubdate`, `source`, `category`, `content`) VALUES (NULL, '".$articleTitle."', '".$thisNews['link']."', '".$thisNews['pubDate']."', '".$thisNews['source_id']."', '".$thisCategory."', '".$output."')";
						if($result = mysqli_query($dbconn, $addSQL))
							echo "ADDED: ".$thisNews['title']."<br>\n";
						else
							echo "ERROR: " . $addSQL . "<br>" . $dbconn->error . "<br>\n";
					}
					else
						echo "ERROR ADDING: ".$thisNews['title']."<br>\n";
				}
				else
				{
					echo "DUPLICATE: ".$thisNews['title']."<br>\n";
				}
			}
		}
	}
	else
	{
		echo $newsJSON['status'].": ".$newsJSON['message']."\n\n";
		exit(1);
	}
	//echo "<pre>\n"; print_r($newsJSON); echo "</pre>\n"; // Show the JSON for testing
}

?>
