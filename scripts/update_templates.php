<?php
// Convert the old ways of storing 'user template' information
// 
// $Id: update_templates.php,v 1.8 2007-02-15 12:08:14 turbo Exp $

// {{{ Setup session etc
require("../include/pql_session.inc");

// Load configuration etc
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_templates.inc");
// }}}

// {{{ Load templates.
$templates = pql_get_templates();
// }}}

// If there are templates (which there SHOULD be since we're called),
// first ADD all the value(s) to all the template(s), then REMOVE
// them from the root DN.
if(is_array($templates)) {
  // {{{ Add value(s) to template(s)
  for($i=0; $i < count($templates); $i++) {
	$dn = urlencode($templates[$i]["dn"]);

	foreach($templates[$i]["oldformat"] as $attrib => $value) {
	  if(pql_modify_attribute($templates[$i]["dn"], $attrib, '', $value))
		$success[] = $attrib;
	  else
		$failed["add"][] = $attrib;
	}
  }
// }}}

  // {{{ Remove the value(s) from the root DN.
  if(is_array($success)) {
	for($i=0; $i < count($success); $i++) {
	  if(!pql_modify_attribute($_SESSION["BASE_DN"][0], $success[$i], '', ''))
		$failed["rem"][] = $attrib;
	}
  }
// }}}

  if(is_array($failed)) {
	echo "Something happened. I did not manage to move all (any?) information from '".$_SESSION["BASE_DN"][0]."' ";
	echo "to the templates object(s)<br>Please review this array of failed attributes and report it at the bugtracker.";
	printr($failed);
	die();
  }
}

// Where to go when we're done...
$url = 'config_detail.php?view=template';

// {{{ We're all done. Back to the config page...
if(pql_get_define("PQL_CONF_DEBUG_ME")) {
  echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<p>";
  die("<b>'".$_SESSION["URI"]."'$url</b>");
} else {
  pql_header($url);
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
