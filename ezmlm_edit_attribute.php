<?php
// $Id: ezmlm_edit_attribute.php,v 1.34 2007-02-15 12:07:11 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
// }}}

// {{{ Forward back to list detail page
function list_forward($domainname, $listno, $msg) {
    $msg    = urlencode($msg);
	$domain = urlencode($_REQUEST["domain"]);
	if($listno)
	  $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&listno=$listno&msg=$msg";
	else
	  $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&msg=$msg";

    pql_header($url);
}
// }}}

// {{{ Get base directory for mails
if(!($path = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
	// TODO: What if we can't find the base maildir path!?
	die(pql_complete_constant($LANG->_('Can\'t get baseMailDir path from %domain%'), array('domain' => $_REQUEST["domain"])));
}
// }}}

// Load list of mailinglists
$user = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
if($ezmlm = new ezmlm($user, $path)) {
	if($ezmlm->mailing_lists[$_REQUEST["listno"]]["name"]) {
		$listname = $ezmlm->mailing_lists[$_REQUEST["listno"]]["name"];
	} else {
		die(pql_complete_constant($LANG->_('No listname defined for list %listnr%'), array('listnr' => $_REQUEST["listno"])));
	}

	// TODO: Same for 'listparent' and 'fromaddress' when/if we need it...
	if(($_REQUEST["type"] == 'subscriber') or ($_REQUEST["type"] == 'owner')) {
		include($_SESSION["path"]."/header.html");

		$type = $_REQUEST["type"];

		if(!@$_REQUEST["submit"] and !$_REQUEST[$type]) {
			if($_REQUEST["type"] == 'subscriber') {
				$title1 = $LANG->_('Add email address to subscription list');
				$title2 = $LANG->_('Subscription address');
			} elseif($_REQUEST["type"] == 'owner') {
				$title1 = $LANG->_('Edit list owner address');
				$title2 = $LANG->_('List owner address');
			}

			// Haven't submitted yet, output the questionaire
?>
  <span class="title1"><?php echo $title1?></span>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $title2?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Email')?></td>
<?php
			if($_REQUEST["type"] == 'subscriber') {
?>
          <td><input type="text" name="subscriber" value="<?php echo $_REQUEST["subscriber"]?>" size="50"></td>
<?php
			} elseif($_REQUEST["type"] == 'owner') {
?>
          <td><input type="text" name="owner" value="<?php echo $_REQUEST["owner"]?>"></td>
<?php
			}
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="listno"     value="<?php echo $_REQUEST["listno"]?>">
    <input type="hidden" name="domain"     value="<?php echo $_REQUEST["domain"]?>">
    <input type="hidden" name="domainname" value="<?php echo $_REQUEST["domainname"]?>">
    <input type="hidden" name="type"       value="<?php echo $_REQUEST["type"]?>">

    <p>

    <input type="submit" name="submit"     value="save">
  </form>
<?php
		} else {
			// Save the value of list owner
			if($_REQUEST["type"] == 'subscriber') {
				if($_REQUEST[$type]) {
					$ezmlm->unsubscribe($listname, $_REQUEST[$type]);
				} else {
					$ezmlm->subscribe($listname, $_REQUEST["subscriber"]);
				}
			} elseif($_REQUEST["type"] == 'owner') {
				$ezmlm->updatelistentry(0, $_REQUEST["listno"], $_REQUEST["domainname"], $_REQUEST["type"],
										$_REQUEST["owner"]);
			}
		}
	} else {
		// Toggle configuration value
		$ezmlm->updatelistentry(0, $_REQUEST["listno"], $_REQUEST["domainname"], $_REQUEST["type"], $ezmlm->mailing_lists[$_REQUEST["listno"]]);
	}
}
?>
  </body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
