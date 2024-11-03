<?php
/*
 * 2024 US Election Results Parser for FujiNet
 * 
 * Copyright (c) 2024 Joe Honold <mozzwald@gmail.com>
 * 
 * License GPL v3
 * 
 * Results scraped from https://www.bloomberg.com/graphics/2024-us-election-results
 */

$votesURL = "live.html";

// Get the HTML content from the target URL/File
$votesRaw = file_get_contents($votesURL);

// Create a new DOMDocument and load the HTML
$votesDOM = new DOMDocument;

// Suppress warnings due to malformed HTML
libxml_use_internal_errors(true);
$votesDOM->loadHTML($votesRaw);
libxml_clear_errors();

// Create an XPath object for the DOMDocument
$xpath = new DOMXPath($votesDOM);

$demCount = 0;
$harrisVotes = 0;
$demSenateSeats = 0;
$demHouseSeats = 0;
$repCount = 0;
$trumpVotes = 0;
$repSenateSeats = 0;
$repHouseSeats = 0;

// Loop through each <span> element that has the classes "party-results win-dem svelte-"
foreach ($votesDOM->getElementsByTagName('span') as $element) {
    // Find the Democrat results
    if (preg_match("#party-results win-dem svelte-#", $element->getAttribute('class'))) {
        if ($demCount == 0)// Presidential is first
        {
            $demCount++;
            // Find the inner <span> that contains 'num' and 'bold' in its class
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $harrisVotes = $resultValue;
            } else {
                $harrisVotes = -1;
            }
        }
        else if ($demCount == 1) // Senate is 2nd
        {
            $demCount++;
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $demSenateSeats = $resultValue;
            } else {
                $demSenateSeats = -1;
            }
        }
        else if ($demCount == 2) // House is third
        {
            $demCount++;
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $demHouseSeats = $resultValue;
            } else {
                $demHouseSeats = -1;
            }
        }
    }

    // Find the Republican results
    if (preg_match("#party-results win-rep svelte-#", $element->getAttribute('class'))) {
        if ($repCount == 0) // Presidential is 1st
        {
            $repCount++;
            // Find the inner <span> that contains 'num' and 'bold' in its class
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $trumpVotes = $resultValue;
            } else {
                $trumpVotes = -1;
            }
        }
        else if ($repCount == 1) // Senate is 2nd
        {
            $repCount++;
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $repSenateSeats = $resultValue;
            } else {
                $repSenateSeats = -1;
            }
        }
        else if ($repCount == 2) // House is third
        {
            $repCount++;
            $numNode = $xpath->query(".//span[contains(@class, 'num') and contains(@class, 'bold')]", $element);
            
            if ($numNode->length > 0) {
                $resultValue = trim($numNode->item(0)->nodeValue);
                    $repHouseSeats = $resultValue;
            } else {
                $repHouseSeats = -1;
            }
        }
    }
}

// Get time/date when the results were scraped
$fileModTime = filemtime("live.html");
$filedate = date("M j, Y", $fileModTime);
$filetime = date("g:ia T", $fileModTime);
//$filedate = date("M j, Y"); // Now
//$filetime = date("g:ia T"); // Now

echo "Harris|" . $harrisVotes . "|Trump|" . $trumpVotes . "|DEMS|" . $demSenateSeats . "|REPS|" . $repSenateSeats . "|DEMH|" . $demHouseSeats . "|REPH|" . $repHouseSeats . "|" . $filedate . "|". $filetime;

?>