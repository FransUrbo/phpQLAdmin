<?php
// control attribute plugin for
// ldapmessagestore (default prefix for mailbox paths)
//
// $Id: attrib.control.ldapmessagestore.inc,v 2.7 2003-05-28 07:23:37 turbo Exp $

function ldapmessagestore_check($type){
	global $error, $ldapmessagestore, $view;

	// missing
	if($ldapmessagestore == ""){
		$error["ldapmessagestore"] = PQL_LANG_MISSING;
		return false;
	}

	// trailing slash ?
	if(!preg_match ("/^(\/)/", $ldapmessagestore)){
		$error["ldapmessagestore"] = PQL_LANG_INVALID;
		return false;
	}

	return true;
}

// init values
function ldapmessagestore_init($host){
	global $_pql_control, $ldapmessagestore, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "ldapmessagestore");
	
	if(!is_null($value)){
		$ldapmessagestore = $value[0];
	}
}

function ldapmessagestore_print_view($host) {
	global $_pql_control, $ldapmessagestore, $view;

	// init data
	ldapmessagestore_init($host);

	// set empty field to "not set"
	if($ldapmessagestore == ""){
		$ldapmessagestore = "not set";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapmessagestore (Default mailbox prefix path)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Path</td>
        <td><?php echo $ldapmessagestore; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapmessagestore"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function ldapmessagestore_print_form(){
	global $attrib, $ldapmessagestore, $PHP_SELF, $error, $mxhost, $view;
	
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapmessagestore (Default mailbox prefix path)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Path</td>
          <td><?php echo format_error($error["ldapmessagestore"]); ?><input type="text" name="ldapmessagestore" value="<?php echo $ldapmessagestore; ?>"></td>
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

function ldapmessagestore_save($type, $host){
	global $_pql_control, $attrib, $ldapmessagestore, $error, $view;
	
	// recontrol values
	if(!ldapmessagestore_check($type)){
 		return false;
	}
	
	switch($type){
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "ldapmessagestore", $ldapmessagestore)){
			$msg = "Successfully saved ldapserver options";
		} else {
			$msg = "Failed saving ldapmessagestore: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}
		
		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function ldapmessagestore_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
?>
	The default prefix for paths in mailmessagestore without trailing / If you set this to /maildisk/ for example, the ldap attributes
	<br><br>
	mailmessagestore: joe/Maildir/
	<br>
  mailmessagestore: /maildisk/joe/Maildir/
	<br><br>
	are equivalent.
	<?php
}

function ldapmessagestore_help_cr(){
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
