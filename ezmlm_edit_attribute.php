<?php
// $Id: ezmlm_edit_attribute.php,v 1.27.2.1 2005-03-04 11:59:45 turbo Exp $
//
require("./include/pql_session.inc");
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

// forward back to list detail page
function list_forward($domainname, $listno, $msg) {
    $msg    = urlencode($msg);
	$domain = urlencode($_REQUEST["domain"]);
	if($listno)
	  $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&listno=$listno&msg=$msg";
	else
	  $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&msg=$msg";

    header("Location: " . $_SESSION["URI"] . "$url");
}

// Get base directory for mails
if(!($path = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die(pql_complete_constant($LANG->_('Can\'t get baseMailDir path from %domain%'), array('domain' => $_REQUEST["domain"])));
}

// Load list of mailinglists
$user = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
if($ezmlm = new ezmlm($user, $path)) {
	if($ezmlm->mailing_lists[$_REQUEST["listno"]]["name"]) {
		$listname = $ezmlm->mailing_lists[$_REQUEST["listno"]]["name"];
	} else {
		die(pql_complete_constant($LANG->_('No listname defined for list %listnr%'), array('listnr' => $_REQUEST["listno"])));
	}

	// TODO: Same for 'listparent' and 'fromaddress' when/if we need it...
	if(($_REQUEST["attrib"] == 'subscriber') or ($_REQUEST["attrib"] == 'owner')) {
		include($_SESSION["path"]."/header.html");

		if(($_REQUEST["submit"] != 'save') and !$value) {
			if($_REQUEST["attrib"] == 'subscriber') {
				$title1 = $LANG->_('Add email address to subscription list');
				$title2 = $LANG->_('Subscription address');
			} elseif($_REQUEST["attrib"] == 'owner') {
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
			if($_REQUEST["attrib"] == 'subscriber') {
?>
          <td><input type="text" name="subscriber" value="<?=$_REQUEST["subscriber"]?>" size="50"></td>
<?php
			} elseif($_REQUEST["attrib"] == 'owner') {
?>
          <td><input type="text" name="owner" value="<?=$_REQUEST["owner"]?>"></td>
<?php
			}
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="listno"     value="<?=$_REQUEST["listno"]?>">
    <input type="hidden" name="domain"     value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="domainname" value="<?=$_REQUEST["domainname"]?>">
    <input type="hidden" name="attrib"     value="<?=$_REQUEST["attrib"]?>">

    <p>

    <input type="submit" name="submit"     value="save">
  </form>
<?php
		} else {
			// Save the value of list owner

			if($_REQUEST["attrib"] == 'subscriber') {
				if($value) {
					$ezmlm->unsubscribe($listname, $value);
				} else {
					$ezmlm->subscribe($listname, $_REQUEST["subscriber"]);
				}
			} elseif($_REQUEST["attrib"] == 'owner') {
				$ezmlm->updatelistentry(0, $_REQUEST["listno"], $_REQUEST["domainname"], $_REQUEST["attrib"],
										$_REQUEST["owner"]);
			}
		}
	} else {
		// Toggle configuration value
		$ezmlm->updatelistentry(0, $_REQUEST["listno"], $_REQUEST["domainname"], $_REQUEST["attrib"], $ezmlm->mailing_lists[$_REQUEST["listno"]]);
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
