# FujiNews

News article fetch & store server for FujiNet

# Installation

1. Make the files available on your webserver
2. Import *database.sql* into mysql
2. Change the API Key, country and language variables in *get_news.php*
3. Change the database username, password and database name in both *get_news.php* and *news.php*
4. Change to directory containing the scripts and install php dependencies: `composer install`
5. Setup a cron job to run *get_news.php* on the schedule of your choosing (recommended once every 15 minutes for *free* newsdata.io api).
