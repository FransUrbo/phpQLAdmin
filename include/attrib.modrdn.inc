<?php
// attribute plugin for changing/renaming the DN
//
// $Id: attrib.modrdn.inc,v 2.20 2007-02-19 14:54:53 turbo Exp $

// {{{ attribute_setup_aci(dn, old, new)
function attribute_setup_aci($dn, $old, $new) {
	$replaced = 0;

	// Get the old ACI's for this object
	$old_acis = pql_aci_get($dn);

	// Look for the user
	for($i=0, $oid=0; $i < count($old_acis); $i++, $oid++) {
		if($old_acis[$i]['SUBJECT']) {
			if(lc($old_acis[$i]['SUBJECT']) == lc($old)) {
				// Replace the old value with the new.
				$new_acis[] = eregi_replace($old, $new, $old_acis[$i]['LINE']);
				$replaced=1;
			} else
			  $new_acis[] = $old_acis[$i]['LINE'];
		} else
		  $new_acis[] = $old_acis[$i]['LINE'];
	}

	// Return the new ACI list.
	if($replaced)
		return($new_acis);
}
// }}}

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;

    // DLW: This doesn't generate an error message.
	if(!empty($_REQUEST["user"]) && !empty($_REQUEST["oldvalue"]) && !empty($_REQUEST["rdn"]))
	  return true;

	return false;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    // DLW: This is weird.  Why isn't oldvalue set to the correct value to begin with?
    $_REQUEST["oldvalue"] = (!empty($_GET["user"]) ? $_GET["user"] : "");
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $LANG, $error;

	$dnparts = split(",", $_REQUEST["oldvalue"]);
	for($i=1; $i < count($dnparts); $i++) {
		$rdn .= $dnparts[$i];
		if($dnparts[$i+1])
		  $rdn .= ",";
	}

	$dnattrs = split("=", $dnparts[0]);
	$len = strlen($dnattrs[1]);
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Rename this RDN')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Old RDN')?></td>
          <td><?=$_REQUEST["oldvalue"]?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('New RDN')?></td>
          <td><b><?=$dnattrs[0]?>=<input type="text" name="user" size="<?=$len+1?>"><b>,<?=$rdn?></b></td>
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
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rdn"      value="<?=$rdn?>">
    <img src="images/info.png" width="16" height="16" border="0">&nbsp;<?=$LANG->_('It is only possible to change the RDN within the same base')?>.
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	// We need TRUE/FALSE value, not ON/OFF...
	if($_REQUEST["keepold"] == 'on')
	  $keepold = 'true';
	else
	  $keepold = 'false';

	// Split the value into two parts - attribute and value
	$dnattr = explode("=", $_REQUEST["oldvalue"]);

	// Create a new DN part
	$user = $dnattr[0]."=".$_REQUEST["user"];

	// {{{ Rename the RDN
	if($_pql->rename($_REQUEST["oldvalue"], $_REQUEST["rdn"], $user, $keepold)) {
	  $msg = pql_complete_constant($LANG->_('Successfully renamed RDN %old% to %new%'),
								   array('old' => $_REQUEST["oldvalue"],
										 'new' => $user));

	  // Currently the $_REQUEST["newrdn"] variable only contain the username.
	  // It is NOT a DN - regenerate the user DN
	  $_REQUEST["user"] = $user.",".$_REQUEST["rdn"];

		// {{{ Replace all references to this user in the admin attributes
		// We're only interested in these attributes
		$attribs = array(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
						 pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
						 pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
						 pql_get_define("PQL_ATTR_SEEALSO"));

		// Replace all administrator/seealso attributes that reference the old
		// user DN with the new DN.
		pql_replace_values($attribs, $_REQUEST["oldvalue"], $user);
		// }}}
		
		// {{{ Change all ACI's in all the objects in all the base DN's.
		if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
			foreach($_SESSION["BASE_DN"] as $dn) {
				$objects = $_pql->get_dn($dn, 'objectClass=*');
				for($i=0; $i < count($objects); $i++) {
					$ACIs = attribute_setup_aci($objects[$i], $_REQUEST["oldvalue"], $user);

					if(is_array($ACIs))
					  // Modify this object. BUG: Should we care about failure/successes?!
					  pql_modify_attribute($objects[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);
				}
			}
		}
		// }}}
	} else {
	  $msg = pql_complete_constant($LANG->_('Failed to rename RDN %user%'),
								   array('user' => $_REQUEST["oldvalue"])) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	  // Currently the $_REQUEST["user"] variable only contain the username.
	  // It is NOT a DN - regenerate the user DN
	  $_REQUEST["user"] = $_REQUEST["oldvalue"];
	}
	// }}}

	// Redirect to the user details page...
	attribute_forward($msg, true);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

