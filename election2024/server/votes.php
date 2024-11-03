<?php
putenv('PUPPETEER_CACHE_DIR=/var/www/.cache');

$output = shell_exec("node /var/www/html/gns/votes.js");

echo "<pre>\n" . $output . "</pre>\n";
?>