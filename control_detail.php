<?php
// Show details on QmailLDAP/Control host
// $Id: control_detail.php,v 1.17 2003-05-15 12:22:42 turbo Exp $
session_start();
require("./include/pql_config.inc");

if($config["PQL_GLOB_CONTROL_USE"]) {
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);

	include("./header.html");

	// Get the values of the mailserver
	$attribs = array("defaultdomain", "plusdomain", "ldapserver",
					 "ldaprebind", "ldapbasedn", "ldapdefaultquota",
					 "ldapdefaultdotmode", "dirmaker", "quotawarning",
					 "locals", "rcpthosts", "ldaplogin", "ldappassword");
	$cn = "cn=" . $mxhost . "," . $USER_SEARCH_DN_CTR;

	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid, $cn, $attrib);
		if(!is_null($value)) {
			if($attrib == "locals") {
				foreach($value as $val) {
					$locals[] = $val;
				}
			} elseif($attrib == "rcpthosts") {
				foreach($value as $val) {
					$rcpthosts[] = $val;
				}
			} elseif($attrib == "ldappassword") {
				$$attrib = "encrypted";
			} else {
				$$attrib = $value[0];
			}
		} else {
			if($attrib == 'ldapserver')
			  $$attrib = "<i>".$USER_HOST."</i>";
			elseif($attrib == 'ldapbasedn')
			  $$attrib = "<i>".$USER_SEARCH_DN_CTR."</i>";
			else
			  $$attrib = "<i>not set</i>";
		}
	}

	// print status message, if one is available
	if(isset($msg)){
		print_status_msg($msg);
	}

	// reload navigation bar if needed
	if(isset($rlnb) and $config["PQL_GLOB_AUTO_RELOAD"]) {
?>

  <script src="frames.js" type="text/javascript" language="javascript1.2"></script>
  <script language="JavaScript1.2"><!--
	// reload navigation frame
	parent.frames.pqlnavctrl.location.reload();
  //--></script>
<?php } ?>

  <span class="title1">Mailserver: <?=$mxhost?></span>

  <br><br>

  <table cellspacing="0" border="0" width="100%" cellpadding="0">
    <tr>
      <td colspan="2" valign="bottom" align="left" width="100%"><a href="<?=$PHP_SELF."?mxhost=$mxhost&view=default"?>"><img alt="/ Base Values \" vspace="0" hspace="0" border="0" src="images/xxx.png"></a><a href="<?=$PHP_SELF."?mxhost=$mxhost&view=hosts"?>"><img alt="/ Locals and RCPT Hosts \" vspace="0" hspace="0" border="0" src="images/xxx.png"></a><a href="<?=$PHP_SELF."?mxhost=$mxhost&view=action"?>"><img alt="/ Action \" vspace="0" hspace="0" border="0" src="images/xxx.png"></a></td>
  </tr>
</table>

<?php
	if(($view == 'default') or ($view == ''))
		include("./tables/control_details-base.inc");

	if($view == 'hosts')
		include("./tables/control_details-hosts.inc");

	if($view == 'action')
		include("./tables/control_details-action.inc");
} else {
?>
  <span class="title1">PQL_GLOB_CONTROL_USE isn't set, won't show control information</span>
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
