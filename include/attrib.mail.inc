<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mail.inc,v 2.22 2004-10-19 10:40:41 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $_pql, $LANG, $error, $defaultdomain;
    
    // is typed in ?
    if(empty($_POST["mail"])) {
		$error[pql_get_define("PQL_ATTR_MAIL")] = $LANG->_('Missing');
		return false;
    }
    
	$mail = $_POST["mail"];
    // Is it REALLY an email address (ie, does it have an "@" in there?)
	// DLW: This change by side-effect should really be in the save code.
    if(!ereg("@", $mail)) {
	  $mail = $mail . "@" . $defaultdomain;
	  $_POST["mail"] = $mail;
	}
    
    // valid
    if(!pql_check_email($mail)) {
		$error[pql_get_define("PQL_ATTR_MAIL")] = $LANG->_('Invalid');
		return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($_POST["oldvalue"] != $mail and pql_email_exists($_pql, $mail)) {
		$error[pql_get_define("PQL_ATTR_MAIL")] = $LANG->_('Already exists');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    // dummy function
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $defaultdomain, $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change main address')?></th>
  <?php
    // We're changing the Main address. Give the old as reference
    if($_REQUEST["oldvalue"] != "") {
?>
<tr class="c2">
  <td class="title"><?=$LANG->_('Old address')?></td>
  <td><?=$_REQUEST["mail"]?></td>
</tr>

<tr class="c2">
  <td class="title"></td>
  <td><input type="checkbox" name="keepold">Add old value as alias</td></td>
</tr>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_ATTR_MAIL")]);
		    ?><input type="text" name="mail" value=""><b>@<?=$defaultdomain?></b></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('An email address must be unique in whole domain')?>
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
		// No add operation possible with mail - 'there can be only one'!
		break;

      case "modify":
		$user = urldecode($_POST["user"]);

		if(pql_modify_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_MAIL"),
									$_POST["oldvalue"], $_POST["mail"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('main address'),
											 'new'  => $_POST["mail"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('main address'))) . ": " . pql_format_error(0);

		if($_POST["keepold"]) {
			// Put the old primary email address as an alias.
			if(pql_modify_attribute($_pql->ldap_linkid, $user, pql_get_define("PQL_ATTR_MAILALTERNATE"),
										'', $_POST["oldvalue"]))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										   array('what' => $LANG->_('main address'),
												 'new'  => $_POST["mail"]));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('main address'))) . ": " . pql_format_error(0);
		}

		// We use the mail attribute to reference users and it has just changed...
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($user, 'attrib.mail.inc:attribute_save')) == pql_get_define("PQL_ATTR_MAIL")) {
			// DLW: $_REQUEST["user"] is used by attribute_forward, and should be a paramater.
			$_REQUEST["user"] = $_POST["mail"];
			// Reload the navigation bar (if allowed by config)
			attribute_forward($msg, true);
		} else {
			attribute_forward($msg);
		}
		
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
