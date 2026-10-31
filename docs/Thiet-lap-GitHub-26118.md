# Kiểm tra và thiết lập GitHub cho dự án 26118

## Issues và Projects đã triển khai ngày 24/10/2026

- [Project BTL 26118 — Tiến độ 8 tuần](https://github.com/users/kimsakota/projects/1), đã liên kết repo BTL và bật public để chia sẻ minh chứng.
- [Cây công việc](https://github.com/users/kimsakota/projects/1/views/1): Issue cha W02 #16 và các sub-issue #17–#20; hierarchy đã bật.
- [Bảng tiến độ](https://github.com/users/kimsakota/projects/1/views/3): cột **Todo → Doing → Review → Done**.
- [Milestone W02](https://github.com/kimsakota/BTL/milestone/1): hạn nội bộ ngày 01/11/2026; thời hạn môn học được ưu tiên nếu sớm hơn.

| Issue | Người phụ trách | Phạm vi | Hạn dự kiến |
|---|---|---|---|
| [W02 #16](https://github.com/kimsakota/BTL/issues/16) | Linh điều phối | Đầu việc cha của tuần | 01/11 |
| [N01 #17](https://github.com/kimsakota/BTL/issues/17) | kimsakota | Baseline, P02 mảng pool, review API | 30/10 |
| [N02 #18](https://github.com/kimsakota/BTL/issues/18) | Minhlth | Review S01, code slab một class | 30/10 |
| [N03 #19](https://github.com/kimsakota/BTL/issues/19) | ankhanh2594 | Hoàn tất môi trường và code harness/assertion | 30/10 |
| [N04 #20](https://github.com/kimsakota/BTL/issues/20) | Cả ba, Linh điều phối | Review, ghép và tổng hợp comment W02 | 31/10 |

Linh là owner; Minh và Khánh đã được cấp quyền **WRITER** cho Project mới. Các Issue đều đang open và ở **Todo**; được gán việc chưa có nghĩa thành viên đã xác nhận nhận việc hoặc đã làm xong.

### Nhận việc và cập nhật minh chứng

1. Mỗi người mở Issue của mình, đọc checklist và phản hồi xác nhận phạm vi khi bắt đầu.
2. Chuyển Todo → Doing khi làm thực tế. Sau buổi có đầu ra, cập nhật việc đã làm, link PR/commit/log, phần còn vướng ngay trên Issue.
3. Chuyển Review khi có đầu ra để kiểm tra; reviewer và hạn dự kiến đã nằm trong bảng. PR ghi `Refs #17` khi liên quan, hoặc `Closes #17` khi đáp ứng đủ điều kiện của Issue tương ứng.
4. Chuyển Done/đóng Issue sau review và kiểm tra thật. Việc chưa xong giữ nguyên ID sang tuần sau; không đổi nhãn thành Done chỉ vì hết tuần.
5. Cuối tuần, mỗi người ghi đóng góp và bằng chứng trực tiếp trên Issue/PR; Linh tổng hợp bằng các permalink comment, PR và log. Không tạo file W02 riêng trong repo. Nếu môn học bắt buộc một file nộp ngoài GitHub thì chỉ tạo theo yêu cầu đó, không coi file đó là nơi theo dõi công việc.

Tuần sau tạo Issue cha W03 và các việc con từ TODO đã chốt, thêm vào cùng Project, chọn Tuần W03 và hạn dự kiến. Không tạo lại Project mỗi tuần; giữ Issue/PR đã hoàn thành để tra lịch sử. Các bước này cập nhật theo công việc thực, không bổ sung hoạt động giả cho những ngày trước.

## Lịch sử kiểm tra trước khi triển khai bảng

Các phần dưới là snapshot từ 19/10 và các lần cập nhật cũ; trạng thái Issues/Projects mới nhất nằm ở phần trên. Không suy ra Settings/CI hiện tại từ snapshot cũ.

Kiểm tra ngày 19/10/2026 trên [kimsakota/BTL](https://github.com/kimsakota/BTL), dùng plugin GitHub và đối chiếu Git local. Cập nhật sau đó: ảnh Linh cung cấp đã hiển thị hai tài khoản với nhãn Collaborator. Cấu hình solo đã áp dụng và kiểm tra lại được ghi ở phần cập nhật bên dưới; clone/push/review của từng bạn và quyền board chưa được xác nhận.

**Bảo vệ main không thay thế việc đưa cả nhóm vào làm.** Hai tài khoản đã có trong danh sách Collaborator theo ảnh mới; tiếp tục clone, đăng lesson để đọc chung, nhận task và review. Minh/Khánh bắt đầu tại [hướng dẫn thành viên](Huong-dan-thanh-vien.md); Linh xem thêm [sổ tay GitHub nhóm](So-tay-GitHub-nhom.md). Connector không hỗ trợ hai endpoint list collaborators/invitations ở lần thử trước; trạng thái thành viên cập nhật dựa trên ảnh, không phải kết quả API.

## 1 Kết quả kiểm tra thực tế

Bảng dưới là snapshot lần audit đầu. **Cập nhật ngày 19/10:** Linh đã commit tài liệu thành `2df489d`, squash merge [PR #1](https://github.com/kimsakota/BTL/pull/1) thành `53872b8` và đồng bộ GitHub Desktop về `main`. [CI trên main](https://github.com/kimsakota/BTL/actions/runs/39212699734) completed/success: `linux` qua format/build/usertests/crash, `macos` qua build. Repo chỉ cho squash merge. Linh đã cài clang-format 21.1.8; thông tin lỗi format và PR đang mở trong bảng bên dưới chỉ mô tả lịch sử audit để tra cứu.

**Bảo vệ main đã xác nhận:** `main-workflow` id `25416827` đã **active**, target default branch, bypass rỗng; PR với 0 approvals, resolve conversations, linear history và chặn xóa/force push. Required checks đã có **`linux`** và **`macos`**, cả hai integration ID `15368`; strict up-to-date tắt. Danh sách checks từng trống ở bước tạo ban đầu, nhưng Linh đã thêm và agent đọc lại được. Đây là lần đọc setting trước ảnh Collaborator; chưa đọc lại approvals hoặc xác nhận board/issues sau khi thêm hai bạn.

| Hạng mục | Kết quả quan sát | Ý nghĩa |
|---|---|---|
| Repository | Public, tài khoản cá nhân `kimsakota`, default branch `main` | Dùng một repo cho nhóm là phù hợp |
| Lịch sử main | 4 commit, nối thẳng, không có merge commit | Chưa rối về đồ thị nhánh |
| Nhánh review | `codex/linh-review-existing-changes`, hơn main 1 commit `7a61ccf` | Local và remote cùng head tại thời điểm kiểm tra |
| Pull request | [PR #1](https://github.com/kimsakota/BTL/pull/1) đang mở, chưa merge, chưa là draft, 17 file | Một checkpoint trộn code, bài học và tổ chức nhóm |
| Issues | Truy vấn issues trả về rỗng | Backlog Markdown chưa được chuyển thành issue |
| Templates | Có trong nhánh PR #1, chưa vào main | Cần đưa lên default branch để áp dụng cho quy trình chung |
| Rule bảo vệ | API branches ghi `main.protected=false`; danh sách rulesets rỗng | Main chưa có các ràng buộc được kiểm tra ở đây |
| Quyền đọc chi tiết protection | Connector trả 403 khi gọi endpoint administration | Không diễn giải 403 thành dữ liệu cấu hình; kết luận trên dựa vào branches/rulesets |
| Merge | Cả merge commit, squash, rebase đều đang bật; tự xóa nhánh sau merge đang tắt | Chưa thống nhất cách main nhận thay đổi |
| GitHub Actions | 5 run được liệt kê đều có conclusion failure | Cần đọc từng job trước khi kết luận code không build |
| Job mới nhất | macOS success; Linux fail tại format, build/test sau đó skipped | Có bằng chứng build từ xa; chưa có bằng chứng runtime từ các job này |
| Tag baseline | Local có `xv6-baseline`; API ref 404, `git ls-remote --tags origin` không có tag | Tag chưa được chia sẻ trên origin |
| Định danh commit | 4 commit cũ không gắn profile; checkpoint mới gắn `kimsakota`; config hiện dùng GitHub noreply đúng | Cấu hình hiện tại đã tốt, không cần sửa lại cùng vấn đề |
| EOL | `.gitattributes` đã có `* text=auto eol=lf` | Không cần thêm một file EOL trùng chức năng |
| Local ngoài task | `VstHelper/` untracked và có `.git` riêng | Đây là repo lồng, cần xác định cách dùng trước khi stage |

Metadata cho biết Issues và Projects được bật, nhưng `has_projects=true` không chứng minh một board Project v2 đã được tạo hay đang được dùng. Lượt này chưa xác minh board v2. Chưa mời thành viên, tạo issue/board, đổi nhánh, commit/push hoặc sửa cấu hình server.

## 2 Commit đang khó đọc ở đâu

| Commit | Nội dung thực tế đã xem | Điểm cần cải thiện |
|---|---|---|
| `e287830` | Nhập xv6 baseline và workspace học | Đây là mốc nền cần giữ |
| `e7279a7` | Tài liệu/bộ nhớ và một thay đổi khoảng trắng `bio.c` | Tên docs không phản ánh toàn bộ phạm vi |
| `8ea6f01` | Lesson 1, tài liệu và chú thích allocator/header | Tên chỉ là `.` |
| `c829ab9` | Lesson 2, bộ nhớ và chú thích/khoảng trắng trong kernel | Tên chỉ là `.`; có thay đổi phụ không liên quan bài học |
| `7a61ccf` trên nhánh PR | 17 file: hàm đếm trang trong 3 file kernel, Lesson 3, quy trình nhóm, báo cáo và templates | Hợp làm checkpoint giữ hiện trạng; quá nhiều mục tiêu cho một PR bàn giao |

**Đề xuất giữ 4 commit cũ trên main và bắt đầu quy tắc rõ từ bây giờ.** Không cần viết lại main để có một quy trình tốt. Đổi tên commit đã push bằng rebase/amend sẽ đổi SHA và cần đồng bộ lại các nhánh đang dựa vào đó; chưa có yêu cầu rõ để thực hiện thao tác ấy.

Đối với PR #1, nên giữ nó như checkpoint tham chiếu, rồi tách các đầu ra để review trước khi merge. Tối thiểu tách phần hàm đếm trang (`defs.h`, `kalloc.c`, `main.c`) khỏi phần tài liệu/quy trình. Nếu tài liệu vẫn quá lớn, tách tiếp Lesson 3 và quy trình nhóm/báo cáo. Nội dung tài liệu mô tả hàm đếm “đã có” cần đi sau PR code tương ứng hoặc ghi đúng trạng thái chờ ghép.

Không cherry-pick nguyên commit `7a61ccf` vào mọi nhánh mới vì sẽ mang lại toàn bộ 17 file. Việc tách/đặt lại PR là bước làm Git riêng cần người dùng yêu cầu rõ; lượt kiểm tra này không thay đổi lịch sử hay PR.

## 3 Cấu hình merge nên dùng

Trong [Settings của repo](https://github.com/kimsakota/BTL/settings), vào General rồi phần Pull Requests:

| Tùy chọn | Hiện tại | Đề xuất |
|---|---|---|
| Allow squash merging | Bật | Giữ bật |
| Allow merge commits | Bật | Tắt để cả nhóm dùng một cách ghép |
| Allow rebase merging | Bật | Tắt trong quy trình ban đầu cho dễ theo dõi |
| Default squash title/message | Dựa vào commit hoặc danh sách commits | Chọn tiêu đề PR và mô tả PR |
| Automatically delete head branches | Tắt | Bật sau khi PR được merge và xác nhận xong |

Mỗi PR nên giải quyết một vấn đề có thể nghiệm thu. Khi squash merge, main nhận một commit cho kết quả PR; các commit làm dở vẫn tra được qua PR. Cần ghi tác giả/co-author thực tế trong mô tả và squash message khi có nhiều người cùng làm. Sau squash merge, công việc mới bắt đầu trên nhánh mới từ main vừa cập nhật. [GitHub về merge methods](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/configuring-pull-request-merges/about-merge-methods-on-github), [cấu hình squash](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/configuring-pull-request-merges/configuring-commit-squashing-for-pull-requests).

Chọn cách merge chỉ làm gọn các thay đổi tiếp theo; không tự đổi tên hai commit `.` đã có trên main.

## 4 Bảo vệ main theo hai giai đoạn

Trong Settings → Rules → Rulesets, Linh đã tạo branch ruleset `main-workflow`, target default branch `main`, enforcement **Active**. Cột solo bên dưới đã áp dụng và đọc lại; cột nhóm là đề xuất khi có reviewer thực sự.

| Quy tắc | Hiện chỉ Linh | Khi Minh hoặc Khánh đã tham gia và có thể review |
|---|---|---|
| Thay đổi qua pull request | Bật | Bật |
| Approval bắt buộc | Tắt hoặc 0 | Một người khác tác giả approve |
| Giải quyết review conversations | Bật | Bật |
| Linear history | Bật với cách squash đã chọn | Giữ bật |
| Chặn force push và xóa main | Bật | Bật |
| Required CI checks | Đã bắt buộc `linux` và `macos`, có run đạt trên main | Giữ các check đã xác nhận |
| Code owner approval | Chưa dùng | Chỉ bật khi có CODEOWNERS đúng tài khoản/quyền và không gây kẹt review |

Không yêu cầu một approval từ người khác khi repo chỉ có tác giả PR. Assistant đọc code không phải một GitHub approving review hợp lệ thay cho thành viên. Ở giai đoạn một người, vẫn tự kiểm tra diff, chạy kiểm tra thực và ghi PR đầy đủ; không gọi đó là review độc lập.

Khi có reviewer, có thể bật dismiss stale approvals khi code thay đổi sau review. Chọn check chính xác từ các run thật, tránh nhập tên check dự định nhưng workflow chưa tạo. GitHub cho phép yêu cầu PR mà không bắt buộc approving review. [GitHub rulesets](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/available-rules-for-rulesets).

Tên check hiện đã kiểm chứng trên main là **`linux`** và **`macos`**, cả hai đạt ở run `39212699734` và đã được thêm vào required checks của ruleset. Enforcement **Active**, target **Include default branch**, Required approvals **0**, Bypass list rỗng, Restrict updates tắt. Cấu hình đã đọc lại sau khi Linh lưu. Linh làm task trên nhánh riêng rồi mở PR; khi thay workflow phải đối chiếu lại tên required checks để tránh chờ một tên không còn được tạo.

Không bật bắt buộc CI đang hỏng trước khi có kế hoạch sửa và chứng minh nó chạy được. Sau khi CI đạt, bật checks để PR chưa đạt không được merge; việc làm đẹp lịch sử không thay thế tiêu chí chất lượng code.

## 5 Sửa CI đúng nguyên nhân

Bằng chứng: [run PR #1 ngày 19/10](https://github.com/kimsakota/BTL/actions/runs/39202934361). Job Linux `117899570299` dừng tại `make fmt && git diff --exit-code`. `make fmt` chạy được; formatter tạo diff ở:

- `kernel/bio.c`: khoảng trắng cuối dòng include.
- `kernel/kalloc.c`: dòng trống, khoảng trắng, canh chú thích và lời gọi có chú thích dài.
- `kernel/main.c`: canh chú thích sau `kinit`.
- `kernel/spinlock.c`: khoảng trắng cuối dòng định nghĩa hàm.
- `kernel/string.c`: khoảng trắng cuối dòng khai báo.

Đây là nguyên nhân quan sát được, không phải suy đoán compiler/QEMU không tương thích. Ubuntu 26.04 và `actions/checkout@v7` đã thực sự chạy trong job; không cần hạ phiên bản chỉ vì tên phiên bản mới.

Linh tự sửa phần định dạng C theo `.clang-format`, giữ nội dung chú thích hữu ích. Với chú thích dài, đặt lời giải thích trên dòng riêng để tránh formatter bẻ lời gọi hàm khó đọc. Kiểm tra diff trước khi nhận thay đổi, rồi tự commit/push. Không tự động xóa chú thích hoặc chạy formatter sửa source thay Linh trong lượt audit.

Các bước kiểm tra Linh thực hiện ở môi trường đã có toolchain:

```text
make fmt
git diff --check
git diff -- kernel/bio.c kernel/kalloc.c kernel/main.c kernel/spinlock.c kernel/string.c
```

Sau khi review/sửa xong, chạy lại formatter và xác nhận không phát sinh diff mới so với phiên bản đã chốt, rồi chạy build/test thích hợp. Gợi ý commit cho phần định dạng riêng: `style(kernel): align learning changes with clang-format`. Phần này chỉ về định dạng, không được tự nhận đã sửa thuật toán hoặc test runtime pass.

CI hiện gộp format → build → usertests → crash vào một job Linux, nên lỗi format che mất thông tin build/test. Đề xuất task CI riêng:

1. Tách check `format` khỏi job `linux-test` để lỗi style không ngăn quan sát build/test của job còn lại; cả hai vẫn cần đạt trước merge khi đã là required checks.
2. Dùng trigger PR vào main và push vào main, giảm chạy trùng cả push nhánh feature và PR như hiện nay. Thêm `workflow_dispatch` nếu muốn chạy tay.
3. Ghi tool versions và timeout; upload log kể cả khi test fail; đặt tên checks ổn định. Chưa dùng số liệu timing CI làm benchmark của đề tài.
4. Giữ job macOS như kiểm tra bổ sung nếu nhóm cần; Linux là môi trường chính để chạy xv6/QEMU theo quy trình nhóm.

Trước khi require `linux-test`, phải tạo job thật và thấy run thật, không coi đây là tên đã tồn tại. Việc chuyển trigger có thể làm required check bị Pending nếu cấu hình bỏ qua PR; xem hướng dẫn [GitHub về triggers và filters](https://docs.github.com/en/actions/how-tos/write-workflows/choose-when-workflows-run/trigger-a-workflow).

**Kết quả cập nhật đã kiểm chứng:** [run39209117349](https://github.com/kimsakota/BTL/actions/runs/39209117349) tại head `58be186` đã completed/success. Linux qua format, build, usertests và crash; macOS qua build. PR kiểm tra merge ref `7e9ea6227d841aa32e1f7d30c0771ee43485c138` ghép head vào main `c829ab9`, chưa phải merge thật. Kết quả này xác nhận các check CI baseline hiện có; chưa có per-CPU/slab hoặc benchmark. Run cũ `39202934361` từng bị format chặn build/test Linux như phần chẩn đoán ở trên.

## 6 Chuẩn hóa commit và task từ bây giờ

Đọc [CONTRIBUTING](../CONTRIBUTING.md) trước khi mở task mới. Mỗi commit nên có một lý do thay đổi, ví dụ:

```text
docs(team): define module ownership and weekly reporting
docs(lesson3): explain per-cpu state and local allocation
feat(page): add locked free-page counting
style(kernel): align learning changes with clang-format
ci: separate formatting from Linux tests
```

Đây là các tên gợi ý theo loại thay đổi, không khẳng định test hoặc tính năng đã hoàn tất. Tránh tên `.` hoặc `update` không có đối tượng; tránh gom thay đổi khoảng trắng ở file không liên quan vào commit thuật toán. Khi đổi chú thích source, vẫn kiểm tra diff và tự commit đúng phạm vi.

Định danh hiện tại đã dùng tên Nguyễn Kim Linh và GitHub noreply khớp profile; commit mới đã gắn tài khoản. Không cần cấu hình lại hoặc rewrite bốn commit cũ để sửa một vấn đề đã được khắc phục cho các commit sau. [GitHub commit email](https://docs.github.com/en/account-and-profile/how-tos/email-preferences/setting-your-commit-email-address).

Issue cần các trường ID, owner, đầu ra, phụ thuộc, tuần, acceptance và evidence. Khi chưa có hai bạn trên GitHub, không assign tài khoản giả; ghi phân công dự kiến trong nội dung. Chỉ mời cộng tác viên khi nhóm chuẩn bị xong và Linh yêu cầu thao tác mời. Board chỉ cần Backlog → Ready → Doing → Review → Done; một task đang bị chặn cần ghi rõ phụ thuộc.

Tag `xv6-baseline` cần được Linh tự push khi muốn chia sẻ mốc nền; kiểm tra tag trỏ đúng commit trước. Lượt audit không push tag. `VstHelper/` có Git riêng: nếu chỉ là mã tham khảo, đề xuất để ở ngoài checkout BTL; nếu thật sự muốn version dependency thì quyết định riêng về submodule. Không stage toàn bộ `VstHelper/` theo thói quen `git add .`.

README và mục lục đã có link tương đối. Lượt hoàn thiện cộng tác đã đổi **94 link nội bộ trong 6 file** (Lesson 0–3, lộ trình học và kiến trúc) từ root ổ D: sang link tương đối; các line suffix của link source đổi thành #L để mở trên GitHub. Con số 108 lần khớp root ở lần quét cũ gồm cả chuỗi đường dẫn minh họa, không chỉ link. Đường dẫn lệnh chạy cần thay theo clone của từng người; nguồn ngoài repo vẫn ghi là nguồn local. Các sửa đổi mới này còn ở máy, chưa được tự commit/push.

## 7 Thứ tự thực hiện đề xuất

| Ưu tiên | Việc | Điều kiện kết thúc |
|---|---|---|
| 1 | Giữ checkpoint PR #1 và xác định scope sẽ bàn giao | Rõ phần code, bài học, quy trình; không mất nội dung |
| 2 | Linh sửa format C; task CI tách check và thu log | CI cho thấy build/test thực sự chạy, kết quả được đọc |
| 3 | Tách phần bàn giao và chốt templates/CONTRIBUTING/link dùng chung | PR nhỏ đúng scope, mô tả/trạng thái nhất quán với source |
| 4 | Chọn squash và ruleset main theo giai đoạn một người — đã áp dụng | PR bắt buộc, 0 approvals, required checks linux/macos |
| 5 | Chia sẻ tag baseline, tạo issues/board cho W1 | Mọi task có ID/đầu ra và link để báo cáo |
| 6 | Thêm Minh/Khánh khi sẵn sàng; bật 1 review | Cả hai clone/build và review được một task nhỏ |

Không cần hoàn tất việc viết lại lịch sử cũ mới được bắt đầu W1. Khi quy trình mới có kiểm tra và các PR đủ nhỏ, main sẽ có các mốc dễ đọc từ đây.

## 8 Phạm vi công việc đã thực hiện trong lượt audit

Đã kiểm tra live repository/branches/commits/PR/issues/rulesets/jobs/log và đối chiếu local; chuẩn bị tài liệu thiết lập, CONTRIBUTING, cập nhật mẫu solo/team, sửa link README và ghi nhớ bằng chứng CI mới. Không sửa C/h/S/Makefile/workflow, không đổi Git config/history/Settings, không stage/commit/push/merge hay mời thành viên. Các chỉnh sửa local mới cần được review và người dùng tự commit nếu quyết định áp dụng.
