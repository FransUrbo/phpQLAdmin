<?php
// attribute plugin for
// deliveryprogrampath (path to program running on delivery)
// $Id: attrib.deliveryprogrampath.inc,v 2.8 2003-07-02 07:59:00 turbo Exp $

function attribute_check($type) {
	global $deliveryprogrampath, $domain, $oldvalue, $_pql, $error, $LANG;
	
	// trailing slash ?
	if(!preg_match ("/^(\/)/", $deliveryprogrampath) and $deliveryprogrampath != "") {
		$error["deliveryprogrampath"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}

function attribute_init() {
	global $deliveryprogrampath, $oldvalue, $_pql;
	global $domain, $user;
	
	$deliveryprogrampath = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_PROGRAM"));
	$deliveryprogrampath = $deliveryprogrampath[0];
	$oldvalue = $deliveryprogrampath;
}

function attribute_print_form() {
	global $deliveryprogrampath, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change path to deliveryprogram')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Change path to deliveryprogram')?></td>
          <td><?php echo format_error($error["deliveryprogrampath"]); ?><input type="text" name="deliveryprogrampath" value="<?=$deliveryprogrampath?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo format_error($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('This is the same as \'|/path/to/someprog\' in a .qmail file. Only used if qmailDotMode is set to ldapwithprog or both')?>.
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
	global $domain, $user, $attrib, $oldvalue, $deliveryprogrampath, $_pql, $view, $LANG;
	
	switch($type) {
	  case "add":
		// no add operation possible with deliveryprogrampath
		break;

	  case "modify":
		if($deliveryprogrampath == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Delivery program path')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_PROGRAM"), $deliveryprogrampath)) {
			if($deliveryprogrampath == "")
			  $deliveryprogrampath = $LANG->_('None');
			
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('path'),
											   'new'  => $deliveryprogrampath));
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('path'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		}
		
		// check if qmaildotmode is set to 'both' or 'ldapwithprog'
		$qmaildotmode = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_DOTMODE"));
		$qmaildotmode = $qmaildotmode[0];
		
		if($qmaildotmode != "both" and $qmaildotmode != "ldapwithprog")
		  $msg .= "<br>" . pql_complete_constant($LANG->_('The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapWithProg to get the deliveryprogram running'),
												 array("qmaildotmode" => $qmaildotmode));
		
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
