<?php
// $Id: ezmlm_edit_attribute.php,v 1.19 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);

// forward back to list detail page
function list_forward($domainname, $msg) {
	global $domain;

    $msg = urlencode($msg);
	// TODO: We need the '$domainname' value to show the LIST, not lists in domain
    $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&msg=$msg&rlnb=3";
    header("Location: " . pql_get_define("PQL_GLOB_URI") . "$url");
}

// Get base directory for mails
if(!($path = pql_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die(pql_complete_constant($LANG->_('Can\'t get baseMailDir path from %domain%'), array('domain' => $domain)));
}

// Load list of mailinglists
if($ezmlm = new ezmlm(pql_get_define("PQL_GLOB_EZMLM_USER"), $path)) {
	if($ezmlm->mailing_lists[$listno]["name"]) {
		$listname = $ezmlm->mailing_lists[$listno]["name"];
	} else {
		die(pql_complete_constant($LANG->_('No listname defined for list %listnr%'), array('listnr' => $listno)));
	}

	// TODO: Same for 'listparent' and 'fromaddress' when/if we need it...
	if(($attrib == 'subscriber') or ($attrib == 'owner')) {
		include("./header.html");

		if(($submit != 'save') and !$value) {
			if($attrib == 'subscriber') {
				$title1 = $LANG->_('Add email address to subscription list');
				$title2 = $LANG->_('Subscription address');
			} elseif($attrib == 'owner') {
				$title1 = $LANG->_('Edit list owner address');
				$title2 = $LANG->_('List owner address');
			}

			// Haven't submitted yet, output the questionaire
?>
  <span class="title1"><?=$title1?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$title2?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
<?php
			if($attrib == 'subscriber') {
?>
          <td><input type="text" name="subscriber" value="<?=$subscriber?>"></td>
<?php
			} elseif($attrib == 'owner') {
?>
          <td><input type="text" name="owner" value="<?=$owner?>"></td>
<?php
			}
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="listno" value="<?=$listno?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="domainname" value="<?=$domainname?>">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="submit" name="submit" value="save">
  </form>
<?php
		} else {
			// Save the value of list owner

			if($attrib == 'subscriber') {
				if($value) {
					$ezmlm->unsubscribe($listname, $value);
				} else {
					$ezmlm->subscribe($listname, $subscriber);
				}
			} elseif($attrib == 'owner') {
				$ezmlm->updatelistentry(0, $listno, $domainname, $attrib, $owner);
			}
		}
	} else {
		// Toggle configuration value
		$ezmlm->updatelistentry(0, $listno, $domainname, $attrib, $ezmlm->mailing_lists[$listno]);
	}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
