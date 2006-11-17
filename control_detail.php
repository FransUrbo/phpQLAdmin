<?php
// Show details on QmailLDAP/Control host
// $Id: control_detail.php,v 1.49.2.5 2006-11-17 16:40:14 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // {{{ Include control api if control is used
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	include($_SESSION["path"]."/header.html");

	if(@$_REQUEST["host"] and !@$_REQUEST["mxhost"])
	  // Called via host_detail.php
	  $_REQUEST["mxhost"] = $_REQUEST["host"];
// }}}

	// {{{ Retreive the actual QLC object if we're called with a physical host (as in via host_detail.php).
	if($_REQUEST["ref"]) {
	  // Get the FQDN of the host
	  $cn = pql_get_attribute($_pql_control->ldap_linkid, $_REQUEST["mxhost"], pql_get_define("PQL_ATTR_CN"));

	  $filter = "(&(".pql_get_define("PQL_ATTR_OBJECTCLASS")."=qmailControl)(cn=$cn))";
	  $result = pql_get_dn($_pql_control->ldap_linkid, $_REQUEST["mxhost"], $filter, 'ONELEVEL');
	  if(is_array($result))
		$_REQUEST["mxhost"] = $result[0];
	  elseif(@$result)
		$_REQUEST["mxhost"] = $result;
	}
// }}}

	// {{{ Get the values of the mailserver
	$attribs = array("defaultdomain"		=> pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					 "plusdomain"			=> pql_get_define("PQL_ATTR_PLUSDOMAIN"),
					 "ldapserver"			=> pql_get_define("PQL_ATTR_LDAPSERVER"),
					 "ldaprebind"			=> pql_get_define("PQL_ATTR_LDAPREBIND"),
					 "ldapcluster"			=> pql_get_define("PQL_ATTR_LDAPCLUSTER"),
					 "ldapbasedn"			=> pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 "ldapdefaultquota"		=> pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"),
					 "defaultquotasize"		=> pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_SIZE"),
					 "defaultquotacount"	=> pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA_COUNT"),
					 "ldapdefaultdotmode"	=> pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"),
					 "dirmaker"				=> pql_get_define("PQL_ATTR_DIRMAKER"),
					 "quotawarning"			=> pql_get_define("PQL_ATTR_QUOTA_WARNING"),
					 "locals"				=> pql_get_define("PQL_ATTR_LOCALS"),
					 "rcpthosts"			=> pql_get_define("PQL_ATTR_RCPTHOSTS"),
					 "ldaplogin"			=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 "ldappassword"			=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
					 "cn"					=> pql_get_define("PQL_ATTR_CN"));
	foreach($attribs as $key => $attrib) {
		$value = pql_get_attribute($_pql_control->ldap_linkid, $_REQUEST["mxhost"], $attrib);
		if($value) {
			if($key == "locals") {
				if(!is_array($value))
				  $value = array($value);
				
				asort($value);
				foreach($value as $val)
				  $locals[] = $val;
			} elseif($key == "rcpthosts") {
				if(!is_array($value))
				  $value = array($value);
				
				asort($value);
				foreach($value as $val)
				  $rcpthosts[] = $val;
			} elseif($key == "ldappassword")
			  $$key = "encrypted";
			else
			  $$key = $value;
		} elseif(!$value) {
			if(($key != "defaultquotasize") and ($key != "defaultquotacount"))
			  $$key = "<i>".$LANG->_('Not set')."</i>";
			else
			  $$key = 0;
		}
	}

	if(isset($defaultquotasize) and isset($defaultquotacount)) {
		$quota = pql_ldap_mailquota(array('maxmails' => $defaultquotacount,
										  'maxsize'  => $defaultquotasize));
	} elseif($ldapdefaultquota)
		$quota = pql_ldap_mailquota(pql_parse_quota($quota));
// }}}

	// {{{ Print status message, if one is available
	if(isset($msg))
	  pql_format_status_msg($msg);
// }}}

	// {{{ Just incase we have a new style quota, but not an old one...
	if(($defaultquotasize and !eregi('not set', $defaultquotasize)) and
	   ($defaultquotacount and !eregi('not set', $defaultquotacount)))
	  $quota = $defaultquotasize."S,".$defaultquotacount."C";
	elseif($ldapdefaultquota and !eregi('not set', $ldapdefaultquota))
	  $quota = $ldapdefaultquota;
// }}}

	// {{{ Reload navigation bar if needed
	if(isset($_REQUEST["rlnb"]) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>

  <script src="tools/frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavctrl.location.reload();
  //--></script>
<?php
	}
// }}}

?>

  <span class="title1">Mailserver: <?=pql_maybe_idna_decode($cn)?></span>

  <br><br>

<?php
	// {{{ Load plugin categories
	require($_SESSION["path"]."/include/pql_control_plugins.inc");
	$cats = pql_plugin_get_cats();
	asort($cats);
// }}}

	// {{{ Setup the host view buttons
	if($_REQUEST["ref"]) {
	  if(@$_REQUEST["view"])
		// Save this so it doesn't disappear
		$tmp = $_REQUEST["view"];

	  $_REQUEST["view"] = 'mailsrv';

	  $buttons = array();

	  if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
		$new = array('hostacl' => 'Host Control');
		$buttons = $buttons + $new;
	  }
	  
	  if(pql_get_define("PQL_CONF_AUTOMOUNT_USE")) {
		$new = array('automount' => 'Automount Information');
		$buttons = $buttons + $new;
	  }
	  
	  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
		$new = array('mailsrv'   => 'Mailserver Administration');
		$buttons = $buttons + $new;
	  }
	  
	  if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
		$new = array('websrv' => 'Webserver Administration');
		$buttons = $buttons + $new;
	  }				 

	  pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"], 'host_detail.php'); echo "    <br>\n";

	  if(@$tmp)
		// Restore the view value
		$_REQUEST["view"] = $tmp;
	  else
		// Unset the view value (so it can be set below)
		unset($_REQUEST["view"]);
	}
// }}}

	// {{{ Setup the controls view buttons
	if(@empty($_REQUEST["cat"]))
	  $_REQUEST["cat"] = 'default';
	if(@empty($_REQUEST["view"]))
	  $_REQUEST["view"] = 'default';

	$buttons = array('default' => 'Base values');
	foreach($cats as $cat) {
	  $new = array(urlencode($cat) => $cat);
	  $buttons = $buttons + $new;
	}

	$new = array('action'  => 'Action');
	$buttons = $buttons + $new;

	pql_generate_button($buttons, "mxhost=".urlencode($_REQUEST["mxhost"])."&ref=".$_REQUEST["ref"]); echo "  <br>\n";
// }}}

	// {{{ Load the requested control details page
	if($_REQUEST["view"] == 'default')
		include("./tables/control_details-base.inc");
	elseif($_REQUEST["view"] == 'hosts')
		include("./tables/control_details-hosts.inc");
	elseif($_REQUEST["view"] == 'simscan')
		include("./tables/domain_details-simscan.inc");
	elseif($_REQUEST["view"] == 'action')
		include("./tables/control_details-action.inc");
	else {
	  // This is ugly. We're called with a 'view' which is actually a 'category'.
	  $url = "control_cat.php?mxhost=".$_REQUEST["mxhost"]."&cat=".urlencode($_REQUEST["view"])."&ref=".$_REQUEST["ref"];
	  pql_header($url);
	}
// }}}
} else {
?>
  <span class="title1">PQL_CONF_CONTROL_USE isn't set, won't show control information</span>
<?php
}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
