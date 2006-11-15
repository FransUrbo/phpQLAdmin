<?php
// Add physical host
// $Id: host_add.php,v 1.1.2.1 2006-11-15 12:59:09 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(isset($_REQUEST['action'])) {
  // {{{ Add new host
  $error = false; $error_text = array();
  $num = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
  if(!isset($_REQUEST['hostip']) || !preg_match("/^$num\\.$num\\.$num\\.$num$/", $_REQUEST['hostip'])) {
    $error = true;
    $error_text["hostip"] = $LANG->_("Invalid IP address");
  }

  if(!isset($_REQUEST['hostname']) || !preg_match("/\w\\.\w/i", $_REQUEST['hostname'])) {
    $error = true;
    $error_text["hostname"] = $LANG->_("Invalid hostname");
  } else {
    // Inputs look good, lets add them
    if(pql_add_computer($_pql->ldap_linkid, $_REQUEST['hostname'], $_REQUEST['hostip'])) {
      $msg = pql_complete_constant($LANG->_("Host %host% added successfully."), array('host' => $_REQUEST['hostname']));
      $url = 'host_detail.php?host=' . urlencode(pql_get_define("PQL_ATTR_CN") . "=" . $_REQUEST['hostname'] . "," . $_SESSION["USER_SEARCH_DN_CTR"]).
	"&msg=$msg&rlnb=1";
    } else {
      $msg = pql_complete_constant($LANG->_("Failed to add %host%"), array('host' => $_REQUEST['hostname']));
      $url = 'host_detail.php?host=Global';
    }

    if(!@empty($_REQUEST["dns"])) {
      // {{{ Create the DNS record
      echo "<b>TODO: Create the DNS record</b><p>";
// }}}
    }

    if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
      echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url: '<b>$url</b>'<br>";
      die($msg);
    } else
      pql_header($url);
  }
// }}} 
}

// {{{ Create the 'Add New Computer' form
require($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="1" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_("Add New Computer")?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("Fully Qualified Domain Name")?></td>
            <td>
              <input type="text" name="hostname" size="40"<?php if(empty($error_text["hostname"])) { ?> value="<?=$_REQUEST['hostname']?>"<?php }?>>
              <?php if(!empty($error_text["hostname"])) { echo pql_format_error_span($error_text["hostname"]); } ?>
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_("IP Address")?></td>
            <td>
              <input type="text" name="hostip" size="20"<?php if(empty($error_text["hostip"])) { ?> value="<?=$_REQUEST['hostip']?>"<?php }?>>
              <?php if(!empty($error_text["hostip"])) { echo pql_format_error_span($error_text["hostip"]); } ?>
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Create DNS object')?></td>
            <td><input type="checkbox" name="dns"<?php if(!@empty($_REQUEST["dns"])) { echo "CHECKED"; } ?>>&nbsp;<?=$LANG->_('Yes')?></td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="view"   value="hostacl">
      <input type="hidden" name="action" value="add_host">
      <input type="submit" name="Submit" value="<?=$LANG->_('Add New Host')?>">
    </form>
<?php
// }}}

/* Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
