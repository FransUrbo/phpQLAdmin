<?php
// attribute plugin for
// ezmlmVirtualUser in a domain/branch
// $Id: attrib.ezmlmuser.inc,v 2.2 2004-06-10 05:37:43 turbo Exp $

function attribute_check() {
    global $error, $LANG;
	return true;
}

function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $LANG->_('EZMLM Virtual User'), 'domain' => $orgname)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Username')?></td>
          <td><?php echo pql_format_error_span($error["ezmlmvirtualuser"]); ?><input type="text" name="ezmlmvirtualuser" value="<?=$_REQUEST["ezmlmvirtualuser"]?>" size="50"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["ezmlmvirtualuser"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;
    
    switch($type) {
      case "add":
      case "modify":
		if($_REQUEST["ezmlmvirtualuser"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = $LANG->_('EZMLM virtual user unchanged');
			attribute_forward($msg);
			break;
    	}

		if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["attrib"], $_REQUEST["ezmlmvirtualuser"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('EZMLM virtual user'),
											 'new'  => $_REQUEST["ezmlmvirtualuser"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('EZMLM virtual user'))) . ": " . ldap_error($_pql->ldap_linkid);
		
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
