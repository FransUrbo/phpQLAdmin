<?php
// control attribute plugin for
// defaultdomain (default domain)
// defaulthost (default hostname)
//
// $Id: attrib.control.defaultdomain.inc,v 2.7 2003-05-28 07:23:37 turbo Exp $

function defaultdomain_check($type) {
	global $error, $defaultdomain, $defaulthost, $plusdomain, $view;

	// check validity
	if($defaultdomain != "" and !check_hostaddress($defaultdomain)){
		$error["defaultdomain"] = PQL_LANG_INVALID;
		$is_error = true;
	}

	// check validity
	if($defaulthost != "" and !check_hostaddress($defaulthost)){
		$error["defaulthost"] = PQL_LANG_INVALID;
		$is_error = true;
	}

	// check validity
	if($plusdomain != "" and !check_hostaddress($plusdomain)){
		$error["plusdomain"] = PQL_LANG_INVALID;
		$is_error = true;
	}

	if($is_error == false){
		return true;
 	}
}

// init values
function defaultdomain_init($host) {
	global $_pql_control, $defaultdomain, $defaulthost, $plusdomain, $view;

	// fetch data from ldap server
	$attribs = array("defaultdomain", "defaulthost", "plusdomain");

	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   $attrib);

		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
}

function defaultdomain_print_view($host) {
	global $_pql_control, $defaultdomain, $defaulthost, $plusdomain, $view;

	// init data
	defaultdomain_init($host);

	// set empty fields to their default value
	if($defaultdomain == "") {
		$defaultdomain = $host . ", (default, taken from 'me')";
	}
	
	if($defaulthost == "") {
		$defaulthost = $host . ", (default, taken from 'me')";
	}
	
	if($plusdomain == "") {
		$plusdomain = $host . ", (default, taken from 'me')";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (Default domain / host)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default domain</td>
        <td><?php echo $defaultdomain; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Default host</td>
        <td><?php echo $defaulthost; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Plus domain</td>
        <td><?php echo $plusdomain; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=defaultdomain"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function defaultdomain_print_form() {
	global $attrib, $defaultdomain, $defaulthost, $plusdomain, $PHP_SELF, $error, $mxhost, $view;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">defaultdomain, defaulthost, plusdomain (Default domain / host)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default domain</td>
          <td><?php echo format_error($error["defaultdomain"]); ?><input type="text" name="defaultdomain" value="<?php echo $defaultdomain; ?>"> Default: <?=$host?> (value in 'me')</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Default host</td>
          <td><?php echo format_error($error["defaulthost"]); ?><input type="text" name="defaulthost" value="<?php echo $defaulthost; ?>"> Default: <?=$host?> (value in 'me')</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Plus domain</td>
          <td><?php echo format_error($error["plusdomain"]); ?><input type="text" name="plusdomain" value="<?php echo $plusdomain; ?>"> Default: <?=$host?> (value in 'me')</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function defaultdomain_save($type, $host) {
	global $_pql_control, $attrib, $defaultdomain, $defaulthost, $plusdomain, $error, $view;

	// recontrol values
	if(!defaultdomain_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
	  case "add";
		// save entries
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "defaultdomain", $defaultdomain);
    	$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "defaulthost", $defaulthost);
		$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
												  "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
												  "plusdomain", $plusdomain);
		
		if(in_array(false, $return)){
			$msg = "Error saving default host / domain options: " . pql_ldap_error($_pql_control->ldap_linkid);
		} else {
    		$msg = "Successfully default host / domain options";
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function defaultdomain_help(){
?>
	<b>Default domain name</b>.<br>
	Default:  me, if that is supplied, otherwise the literal name defaultdomain,
	which is probably not what you want.  qmail-inject adds
	this name to any host name without dots, including
	defaulthost if defaulthost does not have dots.<br>
	(Exception: see plusdomain.)
	<br><br>
	The QMAILDEFAULTDOMAIN environment variable overrides
	defaultdomain.
	<br><br>
	<b>Default host name</b>.  <br>
	Default:  me, if that is supplied, otherwise the literal name defaulthost, which is
	probably not what you want.  qmail-inject adds this
	name to any address without a host name.  defaulthost
	need not be the current host's name.  For example, you
	may prefer that outgoing mail show just your domain
	name.
	<br><br>
	The QMAILDEFAULTHOST environment variable overrides
	defaulthost.
	<br><br>
	<b>Plus domain name.</b>
	<br>Default:  me, if that is supplied;
	otherwise the literal name plusdomain, which is
	probably not what you want.  qmail-inject adds this
	name to any host name that ends with a plus sign,
	including defaulthost if defaulthost ends with a plus
	sign.  If a host name does not have dots but ends with
	a plus sign, qmail-inject uses plusdomain, not
	defaultdomain.
	<br><br>
	The QMAILPLUSDOMAIN environment variable overrides
	plusdomain.
<?php
}

function defaultdomain_help_cr(){
?>
	help text taken from qmail man pages
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
