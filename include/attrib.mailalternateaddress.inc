<?php
// attribute plugin for
// mailalternateaddress (emailaddress alias)
// $Id: attrib.mailalternateaddress.inc,v 2.15 2003-08-15 08:06:17 turbo Exp $

function attribute_check($type) {
    global $mailalternateaddress, $oldvalue, $domain, $_pql, $error, $defaultdomain, $email_domain, $LANG;
    
    // is typed in ?
    if($mailalternateaddress == "") {
		$error["mailalternateaddress"] = $LANG->_('Invalid');
		return false;
    }
    
	// Build the COMPLETE email address
	if(! ereg("@", $mailalternateaddress)) {
		if($email_domain)
		  $mailalternateaddress = $mailalternateaddress . "@" . $email_domain;
		elseif($defaultdomain)
		  $mailalternateaddress = $mailalternateaddress . "@" . $defaultdomain;
	}

    // valid
    if(!check_email($mailalternateaddress)) {
		$error["mailalternateaddress"] = $LANG->_('Invalid');
		return false;
    }
    
    // if type is modify, check existance of old value, else the new one
    if($type == "add") {
		// add
		if(pql_email_exists($_pql, $mailalternateaddress)) {
			$error["mailalternateaddress"] = $LANG->_('Already exists');
			return false;
		}
    } else {
		// modify
		if($oldvalue != $mailalternateaddress and pql_email_exists($_pql, $mailalternateaddress)) {
			$error["mailalternateaddress"] = $LANG->_('Already exists');
			return false;
		}
    }
    
    return true;
}

function attribute_init() {
    // dummy function
}

function attribute_print_form() {
    global $mailalternateaddress, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $defaultdomain, $LANG;
	global $_pql, $view, $rootdn;

	$additionaldomainname = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_ADDITIONALDOMAINNAME"));
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <?php if($oldvalue == ""){echo $LANG->_('Create new alias');}else{echo $LANG->_('Change alias');} ?>
      </th>
<?php
    // We're changing a MailAlternateAddress. Give the old as reference
    if($oldvalue != "") {
		echo "<tr class=\"c2\">";
		echo "  <td class=\"title\">".$LANG->_('Old address')."</td>";
		echo "  <td>" . $mailalternateaddress . "</td>";
		echo "</tr>";
    }
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Email')?></td>
        <td>
          <?php echo format_error($error["mailalternateaddress"]); ?>
          <input type="text" name="mailalternateaddress" value="">
<?php 	if(is_array($additionaldomainname)) { ?>
            <b>@ <select name="email_domain"></b>
              <option value="<?=$defaultdomain?>"><?=$defaultdomain?></option>
<?php		foreach($additionaldomainname as $additional) { ?>
              <option value="<?=$additional?>"><?=$additional?></option>
<?php   	} ?>
            </select>
<?php 	} else { ?>
            <b>@<?=$defaultdomain?></b>
<?php 	} ?>
        </td>
      </tr>

      <tr class="subtitle">
        <td colspan="2">
          <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('An alias must be unique to the domain')?></td>
      </tr>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
    global $domain, $user, $attrib, $oldvalue, $mailalternateaddress, $_pql, $defaultdomain, $email_domain, $view, $LANG;
    
    switch($type) {
      case "add":
		if(pql_modify_userattribute($_pql->ldap_linkid, $user,
									pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"),
									'', $mailalternateaddress))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('alias'),
											 'new'  => $mailalternateaddress));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		if (PQL_CONF_TESTMAIL_AUTOSEND) {
			$url = "user_sendmail.php?email=" . urlencode($mailalternateaddress) . "&domain=$domain&user=" . urlencode($user) . "&msg=" . urlencode($msg);
			header("Location: " . pql_get_define("PQL_GLOB_URI") . $url);
		} else
		  attribute_forward($msg);
		break;

      case "modify";
		if($mailalternateaddress == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('alias')));
			attribute_forward($msg);
			break;
		}
		
		if(pql_modify_userattribute($_pql->ldap_linkid, $user,
									pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"),
									$oldvalue, $mailalternateaddress))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('alias'),
											 'new'  => $mailalternateaddress));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('alias'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		if(pql_get_define("PQL_CONF_TESTMAIL_AUTOSEND")) {
			$url = "user_sendmail.php?email=" . urlencode($mailalternateaddress) . "&domain=$domain&user=" . urlencode($user) . "&msg=" . urlencode($msg);
			header("Location: " . pql_get_define("PQL_GLOB_URI") . $url);
		} else
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
