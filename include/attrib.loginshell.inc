<?php
// attribute plugin for
// loginshell
// $Id: attrib.loginshell.inc,v 2.10 2003-08-18 08:26:11 turbo Exp $

function attribute_check($type) {
    global $loginshell, $oldvalue, $_pql, $error, $LANG;
    
    // is typed in ?
    if($loginshell == "") {
		$error["loginshell"] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}

function attribute_init() {
    global $loginshell, $oldvalue;
    $oldvalue = $loginshell;
}

function attribute_print_form() {
    global $loginshell, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
      
    $shells = pql_get_valid_shells();
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Login shell')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Login shell')?></td>
          <td>
            <?php echo format_error($error["loginshell"]); ?>

            <select name="loginshell">
              <option value="/bin/false" SELECTED>/bin/false</option>
<?php
    foreach($shells as $shell) {
?>
              <option value="<?=$shell?>"><?=$shell?></option>
<?php
    }
?>
            </select>
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
    global $domain, $user, $attrib, $oldvalue, $loginshell, $_pql, $view, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with loginshell
		break;

      case "modify":
		if($loginshell == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('login shell')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, $attrib, $loginshell)) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('login shell'),
											   'new'  => $loginshell));
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('login shell'))) . ": " . ldap_error($_pql->ldap_linkid);
		}
		
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
