<?php
// attribute plugin for changing/renaming the DN
//
// $Id: attrib.modrdn.inc,v 2.11 2004-11-05 12:15:32 turbo Exp $

// {{{ attribute_setup_aci(linkid, dn, old, new)
function attribute_setup_aci($linkid, $dn, $old, $new) {
	// Get the old ACI's for this object
	$old_acis = pql_aci_get($linkid, $dn);

	// Look for the user
	for($i=0, $oid=0; $old_acis[$i]; $i++, $oid++) {
		if($old_acis[$i]['SUBJECT']) {
			if(lc($old_acis[$i]['SUBJECT']) == lc($old)) {
				// Replace the old value with the new.
				$new_acis[] = eregi_replace($old, $new, $old_acis[$i]['LINE']);
			} else
			  $new_acis[] = $old_acis[$i]['LINE'];
		} else
		  $new_acis[] = $old_acis[$i]['LINE'];
	}

	// Return the new ACI list.
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
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rdn"      value="<?=$rdn?>">
    <img src="images/info.png" width="16" height="16" border="0">&nbsp;It is only possible to change the RDN within the same base.
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
	$success = 0;
    if(file_exists("./.DEBUG_ME")) {
		echo "Renaming<br>'<b>".$_REQUEST["oldvalue"]."</b>'<br>to<br>'<b>$user,".$_REQUEST["rdn"]."</b>'<br>";
		$user = $user.",".$_REQUEST["rdn"];

		$success = 1;
	} else {
		if(ldap_rename($_pql->ldap_linkid, $_REQUEST["oldvalue"], $user, $_REQUEST["rdn"], $keepold)) {
			$user = $user.",".$_REQUEST["rdn"];

			$success = 1;
		}
	}
	
	if($success) {
		$msg = pql_complete_constant($LANG->_('Successfully renamed RDN %old% to %new%'),
									 array('old' => $_REQUEST["oldvalue"],
										   'new' => $user));

		// {{{ Replace all references to this user in the admin attributes
		// We're only interested in these attributes
		$attribs = array(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
						 pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
						 pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
						 pql_get_define("PQL_ATTR_SEEALSO"));

		// Replace all administrator/seealso attributes that reference the old
		// user DN with the new DN.
		pql_replace_values($_pql, $attribs, $_REQUEST["oldvalue"], $user);
		// }}}
		
		// {{{ Change all ACI's in all the objects in all the base DN's.
		if($_SESSION["ACI_SUPPORT_ENABLED"] and function_exists("user_generate_aci")) {
			foreach($_pql->ldap_basedn as $dn) {
				$objects = pql_get_dn($_pql->ldap_linkid, $dn, 'objectClass=*');
				for($i=0; $objects[$i]; $i++) {
					$ACIs = attribute_setup_aci($_pql->ldap_linkid, $objects[$i], $_REQUEST["oldvalue"], $user);

					// Modify this object. BUG: Should we care about failure/successes?!
					pql_modify_attribute($_pql->ldap_linkid, $objects[$i], pql_get_define("PQL_ATTR_LDAPACI"), '', $ACIs);
				}
			}
		}
		// }}}
	} else
	  $msg = pql_complete_constant($LANG->_('Failed to rename RDN %user%'),
								   array('user' => $_REQUEST["oldvalue"])) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
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

