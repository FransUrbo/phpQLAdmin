<?php
// attribute plugin for pilotPerson objects etc
// $Id: attrib.outlook.inc,v 1.23.6.1 2004-05-25 13:57:51 turbo Exp $

/*
	Attributes managed here:
	------------------------
	c
	facsimiletelephonenumber
	homePhone
	info
	l
	mobile
	o
	pager
	physicalDeliveryOfficeName
	postalAddress
	postalCode
	postOfficebox
	seealso
	st
	telephoneNumber
	title
	street
	streetAddress
	registeredAddress
	vatNumber

	Not implemented (see no point!)
	-------------------------------
	preferredDeliveryMethod
	physicalDeliveryOfficeName
*/

function attribute_check() {
    global $LANG, $error;

    // is typed in ?
    if(!empty($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }

    return true;
}

function attribute_init() {
    $_REQUEST[$attrib] = (!empty($_GET["oldvalue"]) ? $_GET["oldvalue"] : "");
}

function attribute_print_form() {
    global $url, $LANG, $error;

  // Just to save some typeing.
  $attrib = $_REQUEST["attrib"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="outlook">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Set/Modify attribute %attribute%'), array('attribute' => $attrib)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$attrib?></td>
          <td><?php echo pql_format_error_span($error[$attrib]); ?><?php if($attrib == pql_get_define("PQL_ATTR_INFO")) { ?><textarea name="<?=$attrib?>" rows="10" cols="50"><?=$_REQUEST[$attrib]?></textarea><?php } else { ?><input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="30"></td><?php } ?>
        </tr>
      </th>
    </table>

	<input type="hidden" name="submit"   value="2">
	<input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
	<input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
	<input type="hidden" name="domain"   value="<?=$url["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$url["rootdn"]?>">
	<input type="hidden" name="user"     value="<?=$url["user"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
	<input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

  <script language="JavaScript">
    <!--
    document.outlook.<?=$attrib?>.focus();
    // -->
  </script>
<?php
}

function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
    switch($type) {
      case "add":
		// DLW: Add drops into modify.  Is this an error?
      case "modify":
		if($_REQUEST[$attrib] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $attrib));
			attribute_forward($msg);
			break;
		}

		if($_REQUEST["domain"] and !$_REQUEST["user"]) {
			// Save a domain attribute
			if(($attrib == 'vatnumber') or ($attrib == 'mobile') or ($attrib == 'info')) {
				// Special circumstances - this is availible in the phpQLAdminInfo object class,
				// so we need to call pql_domain_set_value() with an ARRAY with the values to
				// set/modify, not array+value...
				$entry[$attrib] = $_REQUEST[$attrib];
				
				// Retreive old object classes for this domain
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_setup_branch_objectclasses(0, $_REQUEST["domain"], $_pql->ldap_linkid);
				
				// Add the 'phpQLAdminInfo' objectclass
				$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'phpQLAdminInfo';

				if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], NULL, $entry))
				  $success = 1;
			} else {
				if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["attrib"], $_REQUEST[$attrib]))
				  $success = 1;
			}

		} elseif($_REQUEST["user"]) {
			// Save a user attribute
			if(pql_replace_attribute($_pql->ldap_linkid, $_REQUEST["user"], $_REQUEST["attrib"], $_REQUEST[$attrib]))
			  $success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'), array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      case "delete";
		if($_REQUEST["domain"] and !$_REQUEST["user"]) {
			// Save a domain attribute
			if(pql_domain_set_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, ''))
			  $success = 1;
//		} elseif($_REQUEST["user"] and !$_REQUEST["domain"]) {
// Currently no DELETE option
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully deleted %what%'), array('what' => $attrib)) . ".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);

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