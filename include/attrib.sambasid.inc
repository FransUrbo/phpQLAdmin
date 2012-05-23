<?php
// attribute plugin for
// Samba SID
// 

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
    
    // is typed in ?
    if(empty($_REQUEST["sambasid"])) {
		$error[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["sambasid"] = (empty($_GET["sambasid"]) ? "" : $_GET["sambasid"]);
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG, $_pql;

	$sid = pql_get_define("PQL_CONF_SAMBASID", $_REQUEST["rootdn"]);
	if(!$_REQUEST["sambasid"] and $sid) {
	  // Generate a SambaSID
	  $sid .= '-';

	  $uidnr = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_QMAILUID"));
	  if($uidnr)
		$_REQUEST["sambasid"] = (3 * $uidnr);
	}

	if(!$_REQUEST["sambasid"]) {
	  echo pql_format_error_span($LANG->_("No SMB Security ID defined!"));
	  echo "<br>";
	  
	  if($_SESSION["ALLOW_BRANCH_CREATE"]) {
		echo pql_complete_constant($LANG->_("Please see %url% ('SMB Security ID')"),
								   array('url' => "<a href='config_detail.php?view=".urlencode($_REQUEST["rootdn"])."'>phpQLAdmin configuration</a>"));
	  } else {
		echo $LANG->_("Please notify Administrator!");
	  }
	  die();
	}
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Samba SID')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Samba SID')?></td>
          <td>
            <?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_CN")]); ?>
            <?php echo $sid?>
            <input type="text" name="sambasid" value="<?php echo $_REQUEST["sambasid"]?>" size="5">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="sid"      value="<?php echo urlencode($sid)?>">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
    
    switch($type) {
      case "add":
		// no add operation possible with sambasid
		break;

      case "modify":
		$success = 0;

		if($_REQUEST["sid"] and $_REQUEST["sambasid"])
			$_REQUEST["sambasid"] = $_REQUEST["sid"].$_REQUEST["sambasid"];

		if($_REQUEST["sambasid"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Samba SID')));
			attribute_forward($msg);
			break;
    	}
		
		// Make sure that the user have the 'sambaSamAccount' object class!
		$ocs = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_OBJECTCLASS"));
		if(!in_array('sambaSamAccount', $ocs)) {
		  $object = array();
		  $object[pql_get_define("PQL_ATTR_OBJECTCLASS")] = $ocs;
		  $object[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'sambaSamAccount';

		  $attrib = $_REQUEST["attrib"];
		  $object[$attrib] = $_REQUEST["sambasid"];

		  if(pql_modify_attribute($_REQUEST["user"], '', '', $object))
		    $success = 1;
		} else {
		  if($_REQUEST["oldvalue"])
			// Replace specific value.
			$old = $_REQUEST["oldvalue"];
		  else
			// Completely replace the attribute
			$old = 1;
		
		  // Replace the specified attribute
		  if(pql_modify_attribute($_REQUEST["user"], $_REQUEST["attrib"], $old, $_REQUEST["sambasid"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('Samba SID'),
											 'new'  => $_REQUEST["sambasid"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Samba SID')))
			. ":&nbsp;" . ldap_error($_pql->ldap_linkid);

		attribute_forward($msg, true);
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
