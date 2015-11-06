#
# Be sure to run `pod lib lint catapush-ios-sdk-pod.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = "catapush-ios-sdk-pod"
  s.version          = "0.1.0"
  s.summary          = "Add reliable push notifications to your app."
  s.description      = <<-DESC
"Catapush is a simple, reliable and scalable delivery API for transactional push notifications for websites and applications. Ideal for sending data-driven transactional notifications including targeted e-commerce and personalized one-to-one messages."
DESC
  s.homepage         = "https://github.com/Catapush/catapush-ios-sdk-pod"
  # s.screenshots     = "www.example.com/screenshots_1", "www.example.com/screenshots_2"
  s.license          = 'MIT'
  s.author           = { "Alessandro Chiarotto" => "chiarotto.alessandro@gmail.com" }
  s.source           = { :git => "https://github.com/Catapush/catapush-ios-sdk-pod.git", :tag => s.version.to_s }
  s.platform     = :ios, '7.0'
  s.requires_arc = true
  s.source_files = 'Pod/Classes/**/*'
  s.resource_bundles = { 'catapush-ios-sdk-pod' => ['Pod/Assets/*.png'] }
  s.xcconfig = { 'LIBRARY_SEARCH_PATHS' => "$(SRCROOT)/Pods/**", 'OTHER_LDFLAGS' => '-ObjC -lxml2' }
  s.frameworks = 'Security', 'CFNetwork','SystemConfiguration','CoreData'
  s.libraries = 'resolv'
  s.resource = 'CatapushKit/CatapushLibBundle.bundle'
  s.preserve_paths = 'CatapushKit/**/*.*'
  s.vendored_libraries = 'CatapushKit/libCatapushLib.a'

end