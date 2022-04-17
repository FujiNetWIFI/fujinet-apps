# FujiNet Wikipedia Article Viewer

This app uses a custom api on top of the [wikipedia api](https://en.wikipedia.org/w/api.php) to fetch and parse articles from wikipedia.

# Custom API

The custom API is hosted at fujinet.online/8bitwiki/wiki.php. GET requests can be made to this php script using the following options:

 * t: translation mode
   * n=none (default)
   * a=ATASCII
   * cr=carriage return
   * lf=line feed
   * crlf=carriage return & line feed
 * s: search term
 * l: limit search to this many (default 10)
 * a: article pageid from search results
 * p: page number to display (default 1)
 * ps: page size, ColumnsxRows (Default: Atari 39x22)

