<?php
// Attribute plugin for
// maximumDomainUsers
// $Id: attrib.maximummailinglists.inc,v 2.7 2004-02-19 12:39:39 turbo Exp $

function attribute_check($type = '') {
    global $error, $LANG;

	/* DLW: This looks to be a "general" set of functions, but is only called for maximumdomainusers */

    // is typed in ?
    if(empty($_REQUEST["attrib"])) {
		$error[$_REQUEST["attrib"]] = $LANG->_('Missing');
		return false;
    }

	return true;
}

function attribute_print_form($type = "modify") {
    global $error, $LANG;
	global $orgname;			// Set by domain_edit_attributes.php.

    $attrib = $_REQUEST["attrib"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Maximum allowed mailinglists in domain %domain%'), array('domain' => $orgname)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo pql_complete_constant($LANG->_('MMaximum allowed mailinglists in domain %domain%'), array('domain' => $orgname)); ?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
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
    <!-- DLW: Why no "oldvalue" field. -->
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

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

		if(pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed mailinglists'),
											 'domain' => pql_maybe_decode(urlencode($_REQUEST["domain"]))));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what% for domain %domain%'),
									   array('what'   => $LANG->_('maximum allowed mailinglists'),
											 'domain' => pql_maybe_decode(urlencode($_REQUEST["domain"]))))
			. ": " . ldap_error($_pql->ldap_linkid);
	
		attribute_forward(urlencode($msg));
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
