<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximummailinglists.inc,v 2.3 2003-07-02 07:59:01 turbo Exp $

function attribute_check($type = '') {
	global $error, $attrib, $$attrib, $LANG;

    // is typed in ?
    if($attrib == "") {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}

function attribute_print_form($type = "modify") {
	global $error, $attrib, $$attrib, $domain, $rootdn, $view, $orgname, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Maximum allowed mailinglists in domain %domain%'), array('domain' => $orgname)); ?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('Maximum allowed mailinglists in domain %domain%'), array('domain' => $orgname)); ?></td>
          <td>
            <?php echo format_error($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="50">
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?=$LANG->_('A limit of 0 (zero) means unlimited accounts')?>.<br>
              <?=$LANG->_('A negative value means that no mailinglists are allowed')?>.
            </table>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="view"   value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $type, $_pql, $view, $LANG;
	
    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete') {
			$$attrib = '';
		}

		if(pql_set_domain_value($_pql->ldap_linkid, $domain, $attrib, $$attrib))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed mailinglists'),
											 'domain' => $domain));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed mailinglists'),
											 'domain' => $domain)) . ": " . ldap_error($_pql->ldap_linkid);
	
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
