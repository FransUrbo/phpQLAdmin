<?php
// attribute plugin for
// mailmessagestore (path to mailbox)
// $Id: attrib.mailmessagestore.inc,v 2.11 2003-08-15 08:06:17 turbo Exp $

function attribute_check($type) {
	global $mailmessagestore, $domain, $oldvalue, $_pql, $error, $rootdn, $LANG;
	
	// is typed in ?
	if($mailmessagestore == "") {
		$error["mailmessagestore"] = $LANG->_('Missing');
		return false;
	}
	
	// check for absolute paths
	if(!pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $rootdn) and preg_match("/^\//", $mailmessagestore)) {
		$error["general"] = $LANG->_('Absolute paths are denied by configuration') . "<br>";
		return false;
	}
	
	// don't use ".."
	if(preg_match("/(\.){2,}/", $mailmessagestore)) {
		$error["mailmessagestore"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}

function attribute_init() {
	global $mailmessagestore, $oldvalue, $rootdn;
	$mailmessagestore = $oldvalue;
}

function attribute_print_form() {
	global $mailmessagestore, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change path to mailbox')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Change path to mailbox')?></td>
          <td>
            <?php echo format_error($error["mailmessagestore"]); ?>
            <input type="text" name="mailmessagestore" value="<?php echo $mailmessagestore; ?>">
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo format_error($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('If you don\'t provide an absolute path, it will be prefixed with the value in qmail/control/ldapmessagestore. \'..\' (double dotts) are not allowed')?>.
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
	global $domain, $user, $attrib, $oldvalue, $mailmessagestore, $_pql, $view, $LANG;
	
	switch($type) {
	  case "add":
		// no add operation possible with mailmessagestore
		break;

	  case "modify";
		if($mailmessagestore == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Mailbox path')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user,
									 pql_get_define("PQL_GLOB_ATTR_MAILSTORE"),
									 $mailmessagestore))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('mailbox path '),
											 'new'  => $mailmessagestore));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('mailbox path'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
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
