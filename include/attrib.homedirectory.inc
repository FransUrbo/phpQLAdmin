<?php
// attribute plugin for
// homedirectory (path to homedirectory)
// $Id: attrib.homedirectory.inc,v 2.9 2003-08-11 19:49:52 turbo Exp $

function attribute_check($type) {
    global $homedirectory, $domain, $oldvalue, $_pql, $error, $LANG;
    
    // is typed in ?
    if($homedirectory == "") {
		$error["homedirectory"] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $homedirectory)) {
		$error["homedirectory"] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $homedirectory, $oldvalue;
    $homedirectory = $oldvalue;
}

function attribute_print_form() {
    global $homedirectory, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change home directory')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Change home directory')?></td>
          <td><?php echo format_error($error["homedirectory"]); ?><input type="text" name="homedirectory" value="<?=$homedirectory?>" size="50"></td>
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
    global $domain, $user, $attrib, $oldvalue, $homedirectory, $_pql, $view, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with homedirectory
		break;

      case "modify":
		if($homedirectory == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Home directory')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib,$homedirectory))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Home directory', 4),
											 'new'  => $homedirectory));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Home directory', 4))) . ": " . ldap_error($_pql->ldap_linkid);
		
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
