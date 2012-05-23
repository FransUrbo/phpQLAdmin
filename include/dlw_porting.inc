<?php
// To log an message use trigger_error('Hi Mom!', E_USER_WARNING); (or E_USER_ERROR if fatal)

// Wrapper for trigger_error() so that we can
// ignore stuff if we're not running a CVS version
// (which is the only version of phpQLAdmin that
// should output these warnings).
function dlw_trigger_error($msg, $level) {
  if(!preg_match('/CVS/', $_SESSION["VERSION"])) {
    // We're running a released version - don't show varnings!
    return(NULL);
  } else {
    // A CVS version - show warnings!
    trigger_error($msg, $level);
  }
}

//
function dlw_dump_env()
{
  $srcs = array('_GET', '_POST', '_COOKIE', '_SESSION');

  print htmlspecialchars(date("M d H:i:s")) . "<BR>\n";
  foreach ($srcs as $from) {
	global $$from;
	print "<B>$from:</B><BR>\n";
	foreach ($$from as $key => $value) {
	  print "&nbsp;&nbsp;" . htmlspecialchars($key) . ' -&gt; ' . htmlspecialchars($value) . "<BR>\n";
	}
  }
}

// $caller_file, $caller_line are __FILE__ and __LINE__
// required '!', not allowed '-', else optional
function dlw_expect_from($caller_file, $caller_line, $from_name, $varnames, $exclusive = false)
{
  $where = "$caller_file:$caller_line";
  $found = array();
  global $$from_name;
  $from = $$from_name;

  foreach ($from as $key => $value) {
    $found[$key] = 1;
  }

  foreach ($varnames as $vname) {
    if (strpos($vname, '!') !== false && strpos($vname, '!') == 0) {
      $vname = substr($vname, 1);
      if (isset($found[$vname])) {
	unset($found[$vname]);
      } else {
	dlw_trigger_error("$where: Couldn't find required variable: \$$from_name" . "[\"$vname\"]", E_USER_WARNING);
      }
    } else if (strpos($vname, '-') !== false && strpos($vname, '-') == 0) {
      $vname = substr($vname, 1);
      if (isset($found[$vname])) {
	dlw_trigger_error("$where: Found variable that shouldn't be there: \$$from_name" . "[\"$vname\"]", E_USER_WARNING);
      }
    } else {			// Plain old variable name, it's optional.
      unset($found[$vname]);
    }
  }

  if ($exclusive) {
    foreach ($found as $vname => $val) {
      dlw_trigger_error("$where: Extra variables found: \$$from_name" . "[\"$vname\"]", E_USER_WARNING);
    }
  }
}

// I use a really short name here because it gets used multiple times
// in a single function which can hog a lot of horizontal space.
// Get a value, bitching if it comes from an unexpected source.
// Sources are p)ost, g)et, c)ookie, G)lobal and u)nset.
function dlw_v($var_name, $expect_from = false)
{
  if ($expect_from === false)
    $expect_from = 'pg';

  if (isset($_POST[$var_name])) {
    $retval = $_POST[$var_name];
    if (strpos($expect_from, 'p') === false)
      dlw_trigger_error("\"$var_name\" sp(" . htmlspecialchars($_POST[$var_name]) . ") came from _POST", E_USER_WARNING);
  } else if (isset($_GET[$var_name])) {
    $retval = $_GET[$var_name];
    if (strpos($expect_from, 'g') === false)
      dlw_trigger_error("\"$var_name\" sp(" . htmlspecialchars($_GET[$var_name]) . ") came from _GET", E_USER_WARNING);
  } else if (isset($_COOKIE[$var_name])) {
    $retval = $_COOKIE[$var_name];
    if (strpos($expect_from, 'c') === false)
      dlw_trigger_error("\"$var_name\" sp(" . htmlspecialchars($_COOKIE[$var_name]) . ") came from _COOKIE", E_USER_WARNING);
  } else if (isset($_GLOBAL[$var_name])) {
    $retval = $_GLOBAL[$var_name];
    if (strpos($expect_from, 'G') === false)
      dlw_trigger_error("\"$var_name\" sp(" . htmlspecialchars($_GLOBAL[$var_name]) . ") came from _GLOBAL", E_USER_WARNING);
  } else {
    $retval = NULL;
    if (strpos($expect_from, 'u') === false)
      dlw_trigger_error("\"$var_name\" is unset", E_USER_WARNING);
  }

  return $retval;
}
?>
