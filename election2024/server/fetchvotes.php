<?php
putenv('PUPPETEER_CACHE_DIR=/var/www/.cache');

$output = shell_exec("node /var/www/html/2024pres/votes.js");

file_put_contents('live.html', $output);
?>