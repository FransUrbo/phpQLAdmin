<?php
// attribute plugin for
// qmaildotmode (interpretation of .qmail-files)
// $Id: attrib.qmaildotmode.inc,v 2.10 2003-08-15 08:06:17 turbo Exp $

function attribute_check($type) {
	global $qmaildotmode, $domain, $oldvalue, $_pql, $error, $LANG;
	
	// is typed in ?
	if($qmaildotmode == "") {
		$error["qmaildotmode"] = $LANG->_('Missing');
		return false;
	}
	
	// valid entry ?
	switch($qmaildotmode) {
	  case "default":
	  case "both":
	  case "dotonly":
	  case "ldaponly":
	  case "ldapwithprog":
	  case "none":
		break;
	  default:
		$error["qmaildotmode"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}

function attribute_init() {
	global $qmaildotmode, $oldvalue;
	$qmaildotmode = $oldvalue;
}

function attribute_print_form() {
	global $qmaildotmode, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $rootdn, $error, $LANG;
	
	$qmaildotmode_select[$qmaildotmode] = "selected";
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change dotmode')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Change dotmode')?></td>
          <td>
            <?php echo format_error($error["qmaildotmode"]); ?>
            <select name="qmaildotmode">
			  <option value="default" <?=$qmaildotmode_select["default"]?>><?=$LANG->_('Default (taken from ldapDefaultDotMode)')?></option>
			  <option value="both" <?=$qmaildotmode_select["both"]?>><?=$LANG->_('Both (deliveryProgramPath + .qmail)')?></option>
			  <option value="dotonly" <?=$qmaildotmode_select["dotonly"]?>><?=$LANG->_('Dotonly (only .qmail)')?></option>
			  <option value="ldaponly" <?=$qmaildotmode_select["ldaponly"]?>><?=$LANG->_('LDAPOnly (deliveryProgramPath + .qmail ignored for ldap deliveries)')?></option>
			  <option value="ldapwithprog" <?=$qmaildotmode_select["ldapwithprog"]?>><?=$LANG->_('LDAPWithProg (deliveryProgramPath used, .qmail ignored)')?></option>
			  <option value="none" <?=$qmaildotmode_select["none"]?>><?=$LANG->_('None (deliveryProgramPath + .qmail ignored)')?></option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo format_error($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('If you set dotmode to both or ldapwithprog you should add a deliveryprogram too')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $domain, $user, $attrib, $oldvalue, $qmaildotmode, $_pql, $view, $LANG;
	
	// recontrol values
	if(!attribute_check($type))
	  return false;
	
	switch($type) {
	  case "add":
		// no add operation possible with qmaildotmode
		break;

	  case "modify";
		if($qmaildotmode == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Dotmode')));
			attribute_forward($msg);
			break;
    	}
		
		if($qmaildotmode == "default")
		  $qmaildotmode = "";
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user,
									 pql_get_define("PQL_GLOB_ATTR_DOTMODE"),
									 $qmaildotmode))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('dot mode'),
											 'new'  => $qmaildotmode));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('dot mode'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
