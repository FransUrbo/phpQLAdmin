<?php
// attribute plugin for
// base homedirectory for domain
// $Id: attrib.basemaildir.inc,v 2.4 2003-07-02 07:59:00 turbo Exp $

function attribute_check() {
    global $basemaildir, $domain, $oldvalue, $_pql, $error, $LANG;
    
    // is typed in ?
    if($basemaildir == "") {
	$error["basemaildir"] = $LANG->_('Missing');
	return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $basemaildir)) {
	$error["basemaildir"] = $LANG->_('Invalid');
	return false;
    }
    
    return true;
}

function attribute_init() {
    global $basemaildir, $oldvalue;
    $basemaildir = $oldvalue;
}

function attribute_print_form() {
    global $basemaildir, $domain, $PHP_SELF, $attrib, $oldvalue, $error, $view, $orgname, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('default maildirectory base'), 'domain' => $orgname)); ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Mail directory')?></td>
          <td><?php echo format_error($error["basemaildir"]); ?><input type="text" name="basemaildir" value="<?=$basemaildir?>" size="50"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $attrib, $oldvalue, $basemaildir, $_pql, $view, $LANG;
    
    switch($type) {
      case "add":
      case "modify":
		if($basemaildir == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Mailbox path')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $basemaildir))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Path'),
											 'new'  => $basemaildir));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Path'))) . ": " . ldap_error($_pql->ldap_linkid);
		
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
