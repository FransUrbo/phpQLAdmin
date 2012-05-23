<?php
// start page
// $Id: user_search.php,v 2.16 2006-12-16 12:02:09 turbo Exp $
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
include($_SESSION["path"]."/header.html");
?>
  <span class="title2"><?php echo $LANG->_('Find entry/entries whose')?></span>
  <form action="search.php" method="POST">
    <select name="attribute">
      <option value="<?php echo pql_get_define("PQL_ATTR_UID")?>" SELECTED><?=$LANG->_('Username')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_CN")?>"><?=$LANG->_('Common name')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_MAIL")?>"><?=$LANG->_('Mail address')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_MAILHOST")?>"><?=$LANG->_('Mail host')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_FORWARDS")?>"><?=$LANG->_('Forwarding address')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")?>"><?=$LANG->_('DNS Hostname')?></option>
      <option value="<?php echo pql_get_define("PQL_ATTR_ARECORD")?>"><?=$LANG->_('DNS IP Address')?></option>
    </select>

    <select name="filter_type">
      <option value="contains" SELECTED><?php echo $LANG->_('Contains')?></option>
      <option value="is"><?php echo $LANG->_('Is')?></option>
      <option value="starts_with"><?php echo $LANG->_('Starts with')?></option>
      <option value="ends_with"><?php echo $LANG->_('Ends with')?></option>
      <option value="not"><?php echo $LANG->_('Not contains')?></option>
    </select>

    <br><input type="text" name="search_string" size="37">
<?php if($_SESSION["ADVANCED_MODE"]) { ?>
    <br><input type="checkbox" name="debug"><?php echo $LANG->_('Debug search filter')?>
<?php } ?>
    <br><input type="submit" value="<?php echo $LANG->_('Search')?>">
    <br>
    <table cellspacing="0" cellpadding="3" border="0">
      <th>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?php echo $LANG->_('Searching for mail host requires an exact match! However, international characters are ok, they will be decoded accordingly.')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?php echo $LANG->_('Searching for mail address will do a search for mail OR alias.')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?php echo $LANG->_("Searching for mailhost NOT containing x will do a search for: '\u(mail OR alias) AND NOT mailhost\U'")?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?php echo $LANG->_("Searching for a DNS IP Address will force '\uequal\U' ('\uis\U') due to LDAP schema restrictions")?></td>
        </tr>
      </th>
    </table>
  </form>
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
