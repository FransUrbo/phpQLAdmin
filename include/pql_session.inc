<?php
// Manage PHP sessions per phpQLAdmin instance
// $Id: pql_session.inc,v 1.2 2005-03-08 05:40:13 turbo Exp $

// {{{ Start session
if(!ereg('/$', $_SERVER["REQUEST_URI"])) {
  $_SERVER["REQUEST_URI"] .= '/';
}
ini_set("session.cookie_path", $_SERVER["REQUEST_URI"]);
session_start();
// }}}

// {{{ Get 'PWD' so we can find our include files
if($_SERVER["PATH_TRANSLATED"]) {
  $tmp = split('/', $_SERVER["PATH_TRANSLATED"]);
  $count = count($tmp) - 2;

  // Extract the _path_ (not including the file)
  $path = '/';
  for($i=1; $i <= $count; $i++) {
	$path .= $tmp[$i];
	if($tmp[$i+1])
	  $path .= '/';
  }
  $path = preg_replace('/\/$/', '', $path);

  // Remember the path
  $_SESSION["path"] = $path;

  // Remove temporary variables
  unset($path); unset($i); unset($count);
}
// }}}

// {{{ Find out our location in the URL
if(!$_SESSION["URI"]) {
  $tmp1 = preg_quote($_SERVER["DOCUMENT_ROOT"], '/');
  $tmp2 = preg_replace("/$tmp1/", "", $_SESSION["path"]);
  $_SESSION["URI"] = '/'.$tmp2.'/';

  unset($tmp1); unset($tmp2);
}
// }}}

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
