<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximumdomainusers.inc,v 2.9 2003-11-20 08:01:37 turbo Exp $

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
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Maximum allowed users in branch')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Maximum allowed users in branch')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?=$LANG->_('A limit of 0 (zero) means unlimited accounts')?>.
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
									   array('what'   => $LANG->_('maximum allowed users'),
											 'domain' => urldecode($domain)));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed users'),
											 'domain' => urldecode($domain))) . ": " . ldap_error($_pql->ldap_linkid);
	
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
