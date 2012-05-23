<?php
// Manage PHP sessions per phpQLAdmin instance
// $Id: pql_session.inc,v 1.9 2007-02-12 15:34:35 turbo Exp $

//echo "_SERVER: "; echo "<pre>"; print_r($_SERVER); echo "</pre>";
//echo "_SESSION: "; print_r($_SESSION);

// {{{ Check if the user session have expired
if(is_array($_SESSION) and !@$_SESSION["initial_load"] and
   ((!@$_SESSION["USER_DN"] and !@$_SESSION["USER_ID"]) or !@$_SESSION["USER_PASS"]))
{
  $msg = 'Session expired...';
  if(function_exists("http_redirect"))
	http_redirect("index.php?msg=" . urlencode($msg) . "&logout=1&reason=sessexpr");
  else
	header("Location: index.php?msg=" . urlencode($msg) . "&logout=1&reason=sessexpr");
  die();
}
// }}}

// {{{ Get the path in the filesystem
$path = preg_replace("@/include/pql_session.inc@", '', __FILE__);
// }}}

// {{{ Get the path in the URI
$uri = $_SERVER['REQUEST_URI'];
if(preg_match('/\.php/i', $uri) or preg_match('/\.inc/i', $uri) or preg_match('/\.css/i', $uri)) {
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
if(!preg_match('/\/$/', $uri)) {
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
$_SESSION["lynx"]      = (preg_match("/lynx/i", $br))		? 1 : 0;
$_SESSION["konqueror"] = (preg_match("/konqueror/i", $br))	? 1 : 0;
$_SESSION["opera"]     = (preg_match("/opera/i", $br))		? 1 : 0;
$_SESSION["mozilla"]   = (preg_match("/mozilla/i", $br))	? 1 : 0;
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
