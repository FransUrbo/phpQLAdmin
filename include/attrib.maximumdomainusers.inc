<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximumdomainusers.inc,v 2.19 2007-02-15 12:08:07 turbo Exp $

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
    global $error, $LANG;

    $attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Maximum allowed users in branch')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Maximum allowed users in branch')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="text" name="<?php echo $attrib?>" value="<?php echo $_REQUEST[$attrib]?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <img src="images/info.png" width="16" height="16" alt="" border="0" align="left">
            <table>
              <?php echo $LANG->_('A limit of 0 (zero) means unlimited accounts')?>.
            </table>
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
		if($type == 'delete') {
			$_REQUEST[$attrib] = '';
		}

		if(pql_modify_attribute($_REQUEST["domain"], $attrib, 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed users'),
											 'domain' => pql_maybe_decode($_REQUEST["domain"])));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed users'),
											 'domain' => pql_maybe_decode($_REQUEST["domain"])))
			. ": " . ldap_error($_pql->ldap_linkid);
	
		attribute_forward(urlencode($msg));
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
