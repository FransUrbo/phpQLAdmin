<?php
// attribute plugin for
// mailmessagestore (path to mailbox)
// $Id: attrib.mailmessagestore.inc,v 2.18 2004-10-19 10:40:41 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
	
	// is typed in ?
	if(empty($_REQUEST["mailmessagestore"])) {
		$error["mailmessagestore"] = $LANG->_('Missing');
		return false;
	}

	// check for absolute paths
	if(!pql_get_define("PQL_CONF_ALLOW_ABSOLUTE_PATH", $_REQUEST["rootdn"])
	   and preg_match("/^\//", $_REQUEST["mailmessagestore"])) {
		$error["general"] = $LANG->_('Absolute paths are denied by configuration') . "<br>";
		return false;
	}
	
	// don't use ".."
	if(preg_match("/(\.){2,}/", $_REQUEST["mailmessagestore"])) {
		$error["mailmessagestore"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["mailmessagestore"] = $_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?=$_SESSION["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change path to mailbox')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Change path to mailbox')?></td>
          <td>
            <?php echo pql_format_error_span($error["mailmessagestore"]); ?>
            <input type="text" name="mailmessagestore" value="<?php echo $_REQUEST["mailmessagestore"]; ?>">
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('If you don\'t provide an absolute path, it will be prefixed with the value in qmail/control/ldapmessagestore. \'..\' (double dotts) are not allowed')?>.
          </td>
        </tr>
      </th>
	</table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
	
	switch($type) {
	  case "add":
		// no add operation possible with mailmessagestore
		break;

	  case "modify";
		if($_REQUEST["mailmessagestore"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Mailbox path')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_MAILSTORE"),
								'', $_REQUEST["mailmessagestore"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('mailbox path '),
											 'new'  => $_REQUEST["mailmessagestore"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('mailbox path'))) . ": " . pql_format_error(0);
		
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
