<?php
// start page
// home.php,v 1.3 2002/12/13 13:50:12 turbo Exp
//
session_start();
require("./include/pql_config.inc");

include("./header.html");
?>
  <span class="title2"><?php echo PQL_LANG_SEARCH_TITLE; ?></span>
  <form method=post action="search.php">
    <select name=attribute>
      <option value=<?php echo pql_get_define("PQL_GLOB_ATTR_UID"); ?> SELECTED><?php echo PQL_LANG_SEARCH_UID; ?></option>
      <option value=<?php echo pql_get_define("PQL_GLOB_ATTR_CN"); ?>><?php echo PQL_LANG_SEARCH_CN; ?></option>
      <option value=<?php echo pql_get_define("PQL_GLOB_ATTR_MAIL"); ?>><?php echo PQL_LANG_SEARCH_MAIL; ?></option>
      <option value=<?php echo pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"); ?>><?php echo PQL_LANG_SEARCH_MAILALTERNATEADDRESS; ?></option>
      <option value=<?php echo pql_get_define("PQL_GLOB_ATTR_FORWARDS"); ?>><?php echo PQL_LANG_SEARCH_MAILFORWARDINGADDRESS; ?></option>
    </select>

    <select name=filter_type>
      <option value=contains SELECTED><?php echo PQL_LANG_SEARCH_CONTAINS; ?></option>
      <option value=is><?php echo PQL_LANG_SEARCH_IS; ?></option>
      <option value=starts_with><?php echo PQL_LANG_SEARCH_STARTSWITH; ?></option>
      <option value=ends_with><?php echo PQL_LANG_SEARCH_ENDSWITH; ?></option>
    </select>

    <br><input type="text" name="search_string" size="37">
    <br><input type="submit" value="<?php echo PQL_LANG_SEARCH_FINDBUTTON; ?>">
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
