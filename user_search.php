<?php
// start page
// $Id: user_search.php,v 2.10 2005-03-04 11:55:32 turbo Exp $
//
require("./include/pql_session.inc");
require("./include/pql_config.inc");

include($_SESSION["path"]."/header.html");
?>
  <span class="title2"><?=$LANG->_('Find user(s) whose')?></span>
  <form method=post action="search.php">
    <select name="attribute">
      <option value="<?=pql_get_define("PQL_ATTR_UID")?>" SELECTED><?=$LANG->_('Username')?></option>
      <option value="<?=pql_get_define("PQL_ATTR_CN")?>"><?=$LANG->_('Common name')?></option>
      <option value="<?=pql_get_define("PQL_ATTR_MAIL")?>"><?=$LANG->_('Mail address')?></option>
      <option value="<?=pql_get_define("PQL_ATTR_MAILHOST")?>"><?=$LANG->_('Mail host')?></option>
      <option value="<?=pql_get_define("PQL_ATTR_FORWARDS")?>"><?=$LANG->_('Forwarding address')?></option>
    </select>

    <select name="filter_type">
      <option value="contains" SELECTED><?=$LANG->_('Contains')?></option>
      <option value="is"><?=$LANG->_('Is')?></option>
      <option value="starts_with"><?=$LANG->_('Starts with')?></option>
      <option value="ends_with"><?=$LANG->_('Ends with')?></option>
    </select>

    <br><input type="text" name="search_string" size="37">
<?php if($_SESSION["ADVANCED_MODE"]) { ?>
    <br><input type="checkbox" name="debug"><?=$LANG->_('Debug search filter')?>
<?php } ?>
    <br><input type="submit" value="<?=$LANG->_('Find user')?>">
    <br>
    <table cellspacing="0" cellpadding="3" border="0">
      <th>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_('Searching for mail host requires an exact match! However, international characters are ok, they will be decoded accordingly.')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_('Searching for mail address will do a search for mail OR alias.')?></td>
        </tr>
      </th>
    </table>
  </form>
</body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
