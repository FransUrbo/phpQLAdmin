<?php
// attribute plugin for
// mail (standard mail address)
// $Id: attrib.mail.inc,v 2.14.2.1 2003-12-09 16:18:26 dlw Exp $

function attribute_check($type) {
    global $_pql, $LANG, $error, $defaultdomain;
    
    // is typed in ?
    if(empty($_POST["mail"])) {
		$error[pql_get_define("PQL_GLOB_ATTR_MAIL")] = $LANG->_('Missing');
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
		$error[pql_get_define("PQL_GLOB_ATTR_MAIL")] = $LANG->_('Invalid');
		return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($_POST["oldvalue"] != $mail and pql_email_exists($_pql, $mail)) {
		$error[pql_get_define("PQL_GLOB_ATTR_MAIL")] = $LANG->_('Already exists');
		return false;
    }
    
    return true;
}

function attribute_init() {
    // dummy function
}

function attribute_print_form() {
    global $defaultdomain, $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change main address')?></th>
  <?php
    // We're changing the Main address. Give the old as reference
    if($_REQUEST["oldvalue"] != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $_REQUEST["mail"] . "</td>";
		echo "</tr>";
    }
  ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
          <td><?php echo pql_format_error_span($error[pql_get_define("PQL_GLOB_ATTR_MAIL")]);
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
    <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="user"     value="<?=$_REQUEST["user"]?>">
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
		// no add operation possible with mail
		break;

      case "modify":
		if(pql_modify_userattribute($_pql->ldap_linkid, $_POST["user"], pql_get_define("PQL_GLOB_ATTR_MAIL"),
									$_POST["oldvalue"], $_POST["mail"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('main address'),
											 'new'  => $_POST["mail"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('main address'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		// We use the mail attribute to reference users and it has just changed...
		if(pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", pql_get_rootdn($_POST["user"])) == pql_get_define("PQL_GLOB_ATTR_MAIL")) {
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

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
