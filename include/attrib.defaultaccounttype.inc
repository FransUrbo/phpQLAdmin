<?php
// Attribute plugin for
// defaultAccountType
// $Id: attrib.defaultaccounttype.inc,v 2.4 2007-02-15 12:08:05 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type = '') {
    global $error, $LANG;

    // is typed in ?
    if(empty($_REQUEST["attrib"])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
	return true;
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG, $_pql;

	$attrib = $_REQUEST["attrib"];

	// Retreive the template definitions
	require($_SESSION["path"]."/include/pql_templates.inc");
	$templates = pql_get_templates();
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Account type to use if not specifically specified')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Default account type')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <select name="<?php echo $attrib?>">
<?php for($i=0; $i < count($templates); $i++) { ?>
              <option value="<?php echo $templates[$i]["name"]?>"<?php if($templates[$i]["name"] == $_REQUEST["defaultaccounttype"]) { echo " SELECTED"; }?>><?php echo $templates[$i]["short"]?></option>
<?php } ?>
              <option value="alias"><?php echo $LANG->_('Alias object')?></option>
            </select>
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
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];

    switch($type) {
      case "add":
      case "modify":
	  case "delete":
		if($type == 'delete')
		  $_REQUEST[$attrib] = '';

		if(pql_modify_attribute($_REQUEST["domain"], $attrib, 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('default account type'),
											 'domain' => pql_maybe_decode(urldecode($_REQUEST["domain"]))));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('default account type'),
											 'domain' => pql_maybe_decode(urldecode($_REQUEST["domain"])))) . ":&nbsp;".ldap_error($_pql->ldap_linkid);
	
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
