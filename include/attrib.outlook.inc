<?php
// attribute plugin for pilotPerson objects etc
// $Id: attrib.outlook.inc,v 1.34 2007-09-29 21:15:13 turbo Exp $

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

// {{{ attribute_check(void)
function attribute_check() {
    global $LANG, $error;

    // is typed in ?
    if(!empty($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }

    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST[$attrib] = (!empty($_GET["oldvalue"]) ? $_GET["oldvalue"] : "");
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $LANG, $error;

	// Is there an english word for this attribute?
	require($_SESSION["path"]."/include/config_attribute_aliases.inc");
	$ALTERNATE_ATTRIBUTE_NAME = $ATTRIBUTE_ALIAS[$_REQUEST["attrib"]];

	// Just to save some typeing.
	$attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post" name="outlook">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Set/Modify attribute %attribute%'), array('attribute' => $attrib)); ?></th>
        <tr class="<?php pql_format_table(); ?>">
<?php if(@$ALTERNATE_ATTRIBUTE_NAME) { ?>
          <td class="title"><?php echo $ALTERNATE_ATTRIBUTE_NAME?></td>
<?php } else { ?>
          <td class="title"><?php echo $attrib?></td>
<?php } ?>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <?php if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_INFO"))) { ?><textarea name="<?php echo $attrib?>" rows="10" cols="50"><?=$_REQUEST[$attrib]?></textarea><?php } else { ?><input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="30">
          </td><?php } ?>

        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;</td>
          <td><?php echo $LANG->_('Leave empty to remove value')?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>

  <script language="JavaScript">
    <!--
    document.outlook.<?php echo $attrib?>.focus();
    // -->
  </script>
<?php
}

// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
    switch($type) {
      case "add":
		// DLW: Add drops into modify.  Is this an error?
      case "modify":
		if(($_REQUEST[$attrib] == $_REQUEST["oldvalue"]) and ($_REQUEST[$attrib] != '')) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $attrib));
			attribute_forward($msg);
			break;
		}

		if($_REQUEST["domain"] and !$_REQUEST["user"]) {
			// Save a domain attribute

			if(($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_VAT_NUMBER"))) or
			   ($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_MOBILE"))) or
			   ($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_INFO"))))
			{
			  // Special circumstances - an attribute that exists not only in one of my own schemas
			  // (phpQLAdminInfo) but also in other schemas (which takes precedene).

			  // Check if the 'phpQLAdminInfo' object class exists in the domain object.
			  $ocs = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_OBJECTCLASS"));
			  if($ocs and !in_array('phpQLAdminInfo', $ocs)) {
				pql_modify_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_OBJECTCLASS"),
									 '', 'phpQLAdminInfo');
				// If it fails, it fails ... and so will pql_modify_attribute() below.
			  }
			}

			if(pql_modify_attribute($_REQUEST["domain"], $_REQUEST["attrib"], 1, $_REQUEST[$attrib]))
			  $success = 1;
		} elseif($_REQUEST["user"]) {
			// Save a user attribute
			if(pql_modify_attribute($_REQUEST["user"], $_REQUEST["attrib"], 1, $_REQUEST[$attrib]))
			  $success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'), array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'), array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg);
		break;

      case "delete":
		if($_REQUEST["domain"] and !$_REQUEST["user"]) {
			// Delete a domain attribute
			if(pql_modify_attribute($_REQUEST["domain"], $attrib, '', ''))
			  $success = 1;
		} elseif($_REQUEST["user"] and !$_REQUEST["domain"]) {
			// Delete a user attribute
			if(pql_modify_attribute($_REQUEST["user"], $attrib, '', ''))
			  $success = 1;
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
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
