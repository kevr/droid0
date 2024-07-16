if get_option('tests')
  irc_message_test = executable('irc_message_test', 'message.test.cpp',
    dependencies: test_deps)
  test('irc message', irc_message_test)

  irc_client_test = executable('irc_client_test', 'client.test.cpp',
    dependencies: test_deps)
  test('irc client', irc_client_test)
endif
