<?php
// control attribute plugin for
// dirmaker (default prefix for mailbox paths)
//
// $Id: attrib.control.dirmaker.inc,v 2.1 2002-12-17 06:21:05 turbo Exp $

function dirmaker_check($type){
	global $error, $dirmaker;

	// missing
	if($dirmaker == ""){
		$error["dirmaker"] = PQL_MISSING;
		return false;
	}

	// trailing slash ?
	if(!preg_match ("/^(\/)/", $dirmaker)){
		$error["dirmaker"] = PQL_INVALID;
		return false;
	}

	return true;
}

function dirmaker_init($host){
	// init values
	global $_pql_control, $dirmaker;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
									   "dirmaker");

	if(!is_null($value)){
		$dirmaker = $value[0];
	}
}

function dirmaker_print_view($host) {
	global $_pql_control, $dirmaker;

	// init data
	dirmaker_init($host);

	// set empty field to "not set"
	if($dirmaker == ""){
		$dirmaker = "not set";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">dirmaker (Path to homedir creating script)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Path</td>
        <td><?php echo $dirmaker; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=dirmaker"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function dirmaker_print_form(){
	global $attrib, $dirmaker, $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">dirmaker (Path to homedir creating script)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Path</td>
          <td><?php echo format_error($error["dirmaker"]); ?><input type="text" name="dirmaker" value="<?php echo $dirmaker; ?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function dirmaker_save($type, $host){
	global $_pql_control, $attrib, $dirmaker, $error;

	// recontrol values
	if(!dirmaker_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
										 "dirmaker", $dirmaker)){
			$msg = "Successfully saved dirmaker options";
		} else {
			$msg = "Failed saving dirmaker: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function dirmaker_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	If you compiled the autohomedirmake-feature, this contains the FULL path to your script which
	creates missing homedirs. The scrpit is executed under the affected user's uid/gid, so if your
	homedirs are in /home and the homdir for joe (system uid joe) should be created, joe MUST have
	write permissions to /home. This feature is most usefull in virtual user environments where all
	users are mapped to an single system uid/gid pair, let's say virtual/virtual. Then only virtual
	needs write permissions in /home. The script gets the path for the to be created homedir as first
	parameter and aliasempty as second one. A sample script:
	<br>
	<br>
	<pre>
#!/bin/sh
mkdir -m 700 -p $1
	</pre>
	<?php
}

function badmailfrom_help_cr(){
?>
	help text taken from life with qmail-ldap
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
