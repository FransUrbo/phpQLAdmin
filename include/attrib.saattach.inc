<?php
// Attribute plugin for
// simScanAttachmentSuffix
// $Id: attrib.saattach.inc,v 2.4 2007-02-15 12:08:08 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type = '') {
    global $error, $LANG;

    // is typed in ?
    if($_REQUEST["attrib"] == "") {
		$error[$_REQUEST["attrib"]] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG;

    $attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('What attachment suffix to check for virus/spam for domain %domain%'), array('domain' => urldecode(pql_format_urls($_REQUEST["orgname"])))); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Attachment suffix')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="text" name="<?php echo $attrib?>" value="<?php echo $_REQUEST[$attrib]?>" size="10">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="2">
    <input type="hidden" name="attrib" value="<?php echo $attrib?>">
    <input type="hidden" name="domain" value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
	global $error, $attrib, $$attrib, $domain, $rootdn, $_pql, $view, $LANG;

	$attrib = $_REQUEST["attrib"];

    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $_REQUEST[$attrib] = '';

		if(pql_modify_attribute($_REQUEST["domain"], $attrib, '', $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully added %what%'),
									   array('what' => $LANG->_('attachment suffix')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('attachment suffix'))) . ": ".ldap_error($_pql->ldap_linkid);

		if(!pql_get_define("PQL_CONF_DEBUG_ME"))
		  attribute_forward($msg);
		break;
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
