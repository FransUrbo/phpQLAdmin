<?php
// Manage PHP sessions per phpQLAdmin instance
// $Id: pql_session.inc,v 1.1.2.5 2005-05-18 08:27:46 turbo Exp $

// {{{ Get the URI path
// Replace '/' with '\/' so that regexp will work
$root = preg_replace("/\//", "\\/", $_SERVER["DOCUMENT_ROOT"]);

// Remove the document root from _MY_ location.
$uri = preg_replace("/$root/", '', __FILE__);

// Remove the actual path to the file
$uri = '/'.preg_replace("/include\/pql_session.inc/", '', $uri);
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

// Remember our path in the file system.
$_SESSION["path"] = $path;

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
