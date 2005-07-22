<?php
// Manage PHP sessions per phpQLAdmin instance
// $Id: pql_session.inc,v 1.1.2.5.2.1 2005-07-22 10:24:37 turbo Exp $

// {{{ Get the URI path
// Fix the URI Retgathering
$uri = $_SERVER['REQUEST_URI'];
$me  = basename($uri);
$uri = preg_replace("/$me/", '', $uri);
$uri = preg_replace("/\/\//", '/\//', $uri);
if($uri == '//') {
  $uri = '/';
}
// }}}

// {{{ Retreive the protocol and server name
// Replace '/' with '\/' so that regexp will work
$tmp1 = preg_replace("/\//", "\\/", $_SERVER["HTTP_REFERER"]);
$tmp2 = preg_replace("/\//", "\\/", $uri);

// Remove the URI.* part from the HTTP_REFERER
$server = preg_replace("/$tmp2.*/", '', $_SERVER["HTTP_REFERER"]);
// }}}

// {{{ Get 'PWD' so we can find our include files
$path = preg_replace("/include\/pql_session.inc/", '', __FILE__);
// }}}

// {{{ Start session
ini_set("session.cookie_path", $uri);
session_start();
// }}}

// Put together a better and clean URI value
$_SESSION["URI"]  = $server.$uri;

// If you can't login correctly (you get the error
// "Can't find you in the database", try uncomment
// the following two lines...
//print_r($_SESSION);
//die($uri);

// Remember our path in the file system.
$_SESSION["path"] = $path;

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
