<?php
// Manage PHP sessions per phpQLAdmin instance
// $Id: pql_session.inc,v 1.6 2005-09-22 07:32:18 turbo Exp $

//echo "_SERVER: "; echo "<pre>"; print_r($_SERVER); echo "</pre>";

// {{{ Get the path in the filesystem
$path = preg_replace("@/include/pql_session.inc@", '', __FILE__);
// }}}

// {{{ Get the path in the URI
$uri = $_SERVER['REQUEST_URI'];
if(eregi('\.php', $uri) or eregi('\.inc', $uri) or eregi('\.css', $uri)) {
  // '[REQUEST_URI] => /phpQLAdmin-HEAD/index.php'
  // '[REQUEST_URI] => /phpQLAdmin-HEAD/tools/dnszonetemplate.php'
  // '[REQUEST_URI] => /phpQLAdmin-HEAD/control_detail.php?view=Accepted+domains&mxhost=mx1.domain.tld'
  // We'll remove the '/tools/' below...

  // Remove all options included in the URI.
  $uri = preg_replace('/\?.*/', '', $uri);

  // Extract the directory name/path.
  $uri = dirname($uri);
} // else '[REQUEST_URI] => /phpQLAdmin-HEAD/'

//   file `rgrep pql_session.inc . | \
//   sed -e 's@:.*@@' -e 's@\./@@' -e 's@/.*@@' | \
//   sort | uniq` | grep directory
$pattern = array('@/include@',
				 '@/scripts@',
				 '@/tools@');

$uri = preg_replace($pattern, "/", $uri);
if(!ereg('/$', $uri)) {
  $uri .= '/';
}
// }}}

// {{{ Start session
//echo "ini_set('session.cookie_path', '$uri')<br>";
ini_set("session.cookie_path", $uri);
session_start();
// }}}

// {{{ Remember the URI and path values
$_SESSION["URI"]  = $uri;
$_SESSION["path"] = $path;
// }}}

// {{{ Get user agent
$br = strtolower($_SERVER["HTTP_USER_AGENT"]);
$_SESSION["lynx"]      = (eregi("lynx", $br))		? 1 : 0;
$_SESSION["konqueror"] = (eregi("konqueror", $br))	? 1 : 0;
$_SESSION["opera"]     = (eregi("opera", $br))		? 1 : 0;
$_SESSION["mozilla"]   = (eregi("mozilla", $br))	? 1 : 0;
// }}}

// {{{ If you can't login correctly
// (you get the error "Can't find you in the database",
// try uncomment the following two lines...
//echo "_SESSION: <pre>"; print_r($_SESSION); echo "</pre>";
//die("URI: '$uri'");
// }}}

// Unset some variables to un-clutter the global (variable) name space...
unset($br); unset($path); unset($uri); 

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
