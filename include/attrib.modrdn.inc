<?php
// attribute plugin for changing/renaming the DN
//
// $Id: attrib.modrdn.inc,v 2.2 2003-08-18 10:38:39 turbo Exp $

function attribute_check($type) {
	global $user, $oldvalue, $rdn, $view, $domain, $rootdn, $attrib;
	global $PHP_SELF, $LANG;

	if($user && $oldvalue && $rdn)
	  return true;

	return false;
}

function attribute_init() {
	global $user, $oldvalue, $view, $domain, $rootdn;
	$oldvalue = $user;
}

function attribute_print_form() {
	global $oldvalue, $view, $domain, $rootdn, $attrib;
	global $PHP_SELF, $LANG;

	$dnparts = split(",", $oldvalue);
	for($i=1; $dnparts[$i]; $i++) {
		$rdn .= $dnparts[$i];
		if($dnparts[$i+1])
		  $rdn .= ",";
	}

	$len = strlen($dnparts[0]);

	$dnattrs = split("=", $dnparts[0]);
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Rename this RDN')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Old RDN')?></td>
          <td><?=$oldvalue?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('New RDN')?></td>
          <td><b><input type="text" name="user" size="<?=$len+1?>"><b>,<?=$rdn?></b></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Keep old RDN')?></td>
          <td><input type="checkbox" name="keepold">Yes</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <input type="hidden" name="rdn"      value="<?=$rdn?>">
    <img src="images/info.png" width="16" height="16" border="0">&nbsp;It is only possible to change the RDN within the same base.
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $user, $oldvalue, $rdn, $view, $domain, $rootdn, $attrib, $keepold;
	global $PHP_SELF, $LANG, $_pql;

	// We need TRUE/FALSE value, not ON/OFF...
	if($keepold == 'on')
	  $keepold = 'true';
	else
	  $keepold = 'false';

	// Remember it for the fail/success story
	$olduser = $user;

	// Split the value into two parts - attribute and value
	$dnattr = split("=", $user);

	// URL/UTF8 encode the value, not the attribute
	$dn = maybe_encode($dnattr[1], $dnattr[0]);

	// Create a new DN part
	$user = $dnattr[0]."=".$dn;

	// Make sure we can have a ' in the branch
	$oldvalue = eregi_replace("\\\'", "'", $oldvalue);
	$rdn = eregi_replace("\\\'", "'", $rdn);

	if(ldap_rename($_pql->ldap_linkid, $oldvalue, $user, $rdn, $keepold))
	  $msg = pql_complete_constant($LANG->_('Successfully renamed RDN %user%'),
				       array('user' => $olduser));
	else
	  $msg = pql_complete_constant($LANG->_('Failed to rename RDN %user%'),
				       array('user' => $olduser)) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	// Since the DN have change, replace the 'user' value (which is used in the attribute_forward() function).
	$user = $user.",".$rdn;

	// Redirect to the user details page...
	attribute_forward($msg, true);
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>

