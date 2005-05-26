<?php
// Show details on QmailLDAP/Control host
// $Id: control_detail.php,v 1.45 2005-05-26 15:53:14 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // {{{ Include control api if control is used
    include($_SESSION["path"]."/include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	include($_SESSION["path"]."/header.html");
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
					 "nonprimaryrcpthosts"  => pql_get_define("PQL_ATTR_NONPRIMARY_RCPT_HOSTS"));
	$cn = pql_get_define("PQL_ATTR_CN") . "=" . $_REQUEST["mxhost"] . "," . $_SESSION["USER_SEARCH_DN_CTR"];

	foreach($attribs as $key => $attrib) {
		$value = pql_get_attribute($_pql_control->ldap_linkid, $cn, $attrib);
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

  <span class="title1">Mailserver: <?=pql_maybe_idna_decode($_REQUEST["mxhost"])?></span>

  <br><br>

<?php
	// {{{ Setup the buttons
	$buttons = array('default' => 'Base values',
					 'hosts'   => 'Locals and RCPTHosts',
					 'nonprim' => 'Non-primary MX domains');

	if(pql_get_define("PQL_CONF_SIMSCAN_USE")) {
	  $new = array('simscan' => 'SimScan config');
	  $buttons = $buttons + $new;
	}

	$new = array('action'  => 'Action');
	$buttons = $buttons + $new;

	pql_generate_button($buttons, "mxhost=".$_REQUEST["mxhost"]); echo "  <br>\n";
// }}}

	// {{{ Load the requested control details page
	if($_REQUEST["view"] == '')
		$_REQUEST["view"] = 'default';

	if($_REQUEST["view"] == 'default')
		include("./tables/control_details-base.inc");
	elseif($_REQUEST["view"] == 'hosts')
		include("./tables/control_details-hosts.inc");
	elseif($_REQUEST["view"] == 'nonprim')
		include("./tables/control_details-nonprimmx.inc");
	elseif($_REQUEST["view"] == 'simscan')
		include("./tables/domain_details-simscan.inc");
	elseif($_REQUEST["view"] == 'action')
		include("./tables/control_details-action.inc");
// }}}
} else {
?>
  <span class="title1">PQL_CONF_CONTROL_USE isn't set, won't show control information</span>
<?php
}
/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
