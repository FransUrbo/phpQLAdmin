<?php
// attribute plugin for changing/renaming the DN
//
// $Id: attrib.modrdn.inc,v 2.5.2.2 2003-12-15 20:33:07 dlw Exp $

function attribute_check($type) {
    global $error, $LANG;

    // DLW: This doesn't generate an error message.
	if(!empty($_REQUEST["user"]) && !empty($_REQUEST["oldvalue"]) && !empty($_REQUEST["rdn"]))
	  return true;

	return false;
}

function attribute_init() {
	  //global $user, $oldvalue, $view, $domain, $rootdn;
	  //$oldvalue = $user;
    // DLW: This is weird.  Why isn't oldvalue set to the correct value to begin with?
    $_REQUEST["oldvalue"] = (!empty($_GET["user"]) ? $_GET["user"] : "");
}

function attribute_print_form() {
    global $LANG, $error;

	$dnparts = split(",", $_REQUEST["oldvalue"]);
	for($i=1; $dnparts[$i]; $i++) {
		$rdn .= $dnparts[$i];
		if($dnparts[$i+1])
		  $rdn .= ",";
	}

	$len = strlen($dnparts[0]);

	$dnattrs = split("=", $dnparts[0]);
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Rename this RDN')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Old RDN')?></td>
          <td><?php echo pql_maybe_decode($_REQUEST["oldvalue"]); ?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('New RDN')?></td>
          <td><b><input type="text" name="user" size="<?=$len+1?>"><b>,<?=$rdn?></b></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Keep old RDN')?></td>
          <td><input type="checkbox" name="keepold">Yes</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rdn"      value="<?=$rdn?>">
    <img src="images/info.png" width="16" height="16" border="0">&nbsp;It is only possible to change the RDN within the same base.
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;

	// We need TRUE/FALSE value, not ON/OFF...
	if($_REQUEST["keepold"] == 'on')
	  $keepold = 'true';
	else
	  $keepold = 'false';

	// Remember it for the fail/success story
	$olduser = $_REQUEST["user"];

	// Split the value into two parts - attribute and value
	$dnattr = split("=", $_REQUEST["user"]);

	// URL/UTF8 encode the value, not the attribute
	$dn = pql_maybe_encode($dnattr[1], $dnattr[0]);

	// Create a new DN part
	$user = $dnattr[0]."=".$dn;

	// Make sure we can have a ' in the branch
	$oldvalue = eregi_replace("\\\'", "'", $_REQUEST["oldvalue"]);
	$rdn = eregi_replace("\\\'", "'", $_REQUEST["rdn"]);

	if(ldap_rename($_pql->ldap_linkid, $oldvalue, $user, $rdn, $keepold)) {
		$msg = pql_complete_constant($LANG->_('Successfully renamed RDN %user%'),
									 array('user' => $olduser));

		// Since the DN have change, replace the 'user' value (which is used in
		// the attribute_forward() function later on).
		$user = $user.",".$rdn;

		// Replace all administrator/seealso attributes that reference the old
		// user DN with the new DN.
		pql_replace_admins($_pql, $oldvalue, $user);
		// DLW: This should force a reload of the ldap user index on the left side.
	} else
	  $msg = pql_complete_constant($LANG->_('Failed to rename RDN %user%'),
				       array('user' => $olduser)) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	// Redirect to the user details page...
	attribute_forward($msg, true);
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

