<?php
// Show details on QmailLDAP/Control host
// $Id: control_detail.php,v 1.29 2004-03-11 18:13:32 turbo Exp $
session_start();
require("./include/pql_config.inc");

if(pql_get_define("PQL_CONF_CONTROL_USE")) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

	include("./header.html");

	// Get the values of the mailserver
	$attribs = array(pql_get_define("PQL_ATTR_DEFAULTDOMAIN"),
					 pql_get_define("PQL_ATTR_PLUSDOMAIN"),
					 pql_get_define("PQL_ATTR_LDAPSERVER"),
					 pql_get_define("PQL_ATTR_LDAPREBIND"),
					 pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 pql_get_define("PQL_ATTR_LDAPDEFAULTQUOTA"),
					 pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"),
					 pql_get_define("PQL_ATTR_DIRMAKER"),
					 pql_get_define("PQL_ATTR_QUOTA_WARNING"),
					 pql_get_define("PQL_ATTR_LOCALS"),
					 pql_get_define("PQL_ATTR_RCPTHOSTS"),
					 pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 pql_get_define("PQL_ATTR_LDAPPASSWORD"));
	$cn = pql_get_define("PQL_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"];

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid, $cn, $attrib);
		if(!is_null($value)) {
			if($attrib == pql_get_define("PQL_ATTR_LOCALS")) {
				asort($value);
				foreach($value as $val) {
					$locals[] = $val;
				}
			} elseif($attrib == pql_get_define("PQL_ATTR_RCPTHOSTS")) {
				asort($value);
				foreach($value as $val) {
					$rcpthosts[] = $val;
				}
			} elseif($attrib == pql_get_define("PQL_ATTR_LDAPPASSWORD")) {
				$$attrib = "encrypted";
			} else {
				$$attrib = $value[0];
			}
		} else {
			if($attrib == pql_get_define("PQL_ATTR_LDAPSERVER"))
			  $$attrib = "<i>".$_SESSION["USER_HOST"]."</i>";
			elseif($attrib == pql_get_define("PQL_ATTR_LDAPBASEDN"))
			  $$attrib = "<i>".$_SESSION["USER_SEARCH_DN_CTR"]."</i>";
			else
			  $$attrib = "<i>".$LANG->_('Not set')."</i>";
		}
	}

	// print status message, if one is available
	if(isset($msg)) {
		pql_format_status_msg($msg);
	}

	// reload navigation bar if needed
	if(isset($rlnb) and pql_get_define("PQL_CONF_AUTO_RELOAD")) {
?>

  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavctrl.location.reload();
  //--></script>
<?php } ?>

  <span class="title1">Mailserver: <?=pql_maybe_idna_decode($mxhost)?></span>

  <br><br>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><a href="<?=$_SERVER["PHP_SELF"]."?mxhost=$mxhost&view=default"?>"><img alt="/ Base Values \" vspace="0" hspace="0" border="0" src="navbutton.php?Base Values"></a><a href="<?=$_SERVER["PHP_SELF"]."?mxhost=$mxhost&view=hosts"?>"><img alt="/ Locals and RCPT Hosts \" vspace="0" hspace="0" border="0" src="navbutton.php?Locals and RCPT Hosts"></a><br><a href="<?=$_SERVER["PHP_SELF"]."?mxhost=$mxhost&view=action"?>"><img alt="/ Action \" vspace="0" hspace="0" border="0" src="navbutton.php?Action"></a></td>
  </tr>
</table>

<?php
	if($view == '')
		$view = 'default';

	if($view == 'default')
		include("./tables/control_details-base.inc");

	if($view == 'hosts')
		include("./tables/control_details-hosts.inc");

	if($view == 'action')
		include("./tables/control_details-action.inc");
} else {
?>
  <span class="title1">PQL_CONF_CONTROL_USE isn't set, won't show control information</span>
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
