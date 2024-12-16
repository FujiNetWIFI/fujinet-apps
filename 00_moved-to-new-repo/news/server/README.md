# FujiNews

News article fetch & store server for FujiNet

## Installation

1. Make the files available on your webserver
2. Import *database.sql* into mysql
2. Change the API Key, country and language variables in *get_news.php*
3. Change the database username, password and database name in both *get_news.php* and *news.php*
4. Change to directory containing the scripts and install php dependencies: `composer install`
5. Setup a cron job to run *get_news.php* on the schedule of your choosing (recommended once per hour for *free* newsdata.io api)

## Using the Fujinews API

Category listings and articles are accessed using *news.php* by passing *GET* parameters to it. These are the supported parameters:

 * General:
  * t: translation mode (line endings and text conversion)
   * n=none (default)
   * a=ATASCII
   * cr=carriage return
   * lf=line feed
   * crlf=carriage return & line feed
  * ps: page size, ColumnsxRows (default 39x22)
   * this is the screen area available for displaying text
   * number of article pages is calculated based on this
  * p: page number to display (default 1)
 
  * Category:
   * c: category name:
    * top, world, science, business, technology,
    * health, entertainment, politics, sports
   * l: limit results to this many (default 10)
 
  * Article:
   * a: article ID from search results

If an article ID and category are both supplied, the category is ignored and the article will be returned.

## Category Results

Category results are returned as 'pages' that are calculated based on the set limit. The first line of category results contains the current page number and total number of pages (ex: *2/9*). The remaining lines contain articles in that category. Each article is on it's own line with the following information separated with pipe character:

Article ID (ex: *1234*) | Date/Time (ex: *2022-05-09 00:28:14*) | Article Title (ex: *FujiNet is the coolest retrocomputer accessory!*)

## Article Results

The first four lines of a returned article contain the following information:

 1. Article Title (ex: *FujiNet is the coolest retrocomputer accessory!*)
 2. Date/Time (ex: *2022-05-09 00:28:14*)
 3. Source (ex: *nytimes*)
 4. Current page/Total pages (ex: *3/14*)

The remaining lines are the article content sized to fit the set page size.
