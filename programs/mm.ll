; ModuleID = '../programs/mm.c'
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv8-none--eabi"

@main.mat2 = private unnamed_addr constant [4 x [4 x i32]] [[4 x i32] [i32 1, i32 1, i32 1, i32 1], [4 x i32] [i32 2, i32 2, i32 2, i32 2], [4 x i32] [i32 3, i32 3, i32 3, i32 3], [4 x i32] [i32 4, i32 4, i32 4, i32 4]], align 4
@.str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str = private unnamed_addr constant [18 x i8] c"Result matrix is \00"

; Function Attrs: nounwind
define void @multiply([4 x i32]* nocapture readonly %mat1, [4 x i32]* nocapture readonly %mat2, [4 x i32]* nocapture %res) #0 {
entry:
  %arrayidx11 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 0, i32 0
  %arrayidx11.1 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 1, i32 0
  %arrayidx11.2 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 2, i32 0
  %arrayidx11.3 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 3, i32 0
  %arrayidx11.1.39 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 0, i32 1
  %arrayidx11.1.1 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 1, i32 1
  %arrayidx11.2.1 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 2, i32 1
  %arrayidx11.3.1 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 3, i32 1
  %arrayidx11.2.42 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 0, i32 2
  %arrayidx11.1.2 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 1, i32 2
  %arrayidx11.2.2 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 2, i32 2
  %arrayidx11.3.2 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 3, i32 2
  %arrayidx11.3.45 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 0, i32 3
  %arrayidx11.1.3 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 1, i32 3
  %arrayidx11.2.3 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 2, i32 3
  %arrayidx11.3.3 = getelementptr inbounds [4 x i32], [4 x i32]* %mat2, i32 3, i32 3
  br label %for.cond.1.preheader

for.cond.1.preheader:                             ; preds = %for.cond.1.preheader, %entry
  %i.037 = phi i32 [ 0, %entry ], [ %inc18, %for.cond.1.preheader ]
  %arrayidx9 = getelementptr inbounds [4 x i32], [4 x i32]* %mat1, i32 %i.037, i32 0
  %arrayidx4 = getelementptr inbounds [4 x i32], [4 x i32]* %res, i32 %i.037, i32 0
  store i32 0, i32* %arrayidx4, align 4, !tbaa !3
  %0 = load i32, i32* %arrayidx9, align 4, !tbaa !3
  %1 = load i32, i32* %arrayidx11, align 4, !tbaa !3
  %mul = mul nsw i32 %1, %0
  store i32 %mul, i32* %arrayidx4, align 4, !tbaa !3
  %arrayidx9.1 = getelementptr inbounds [4 x i32], [4 x i32]* %mat1, i32 %i.037, i32 1
  %2 = load i32, i32* %arrayidx9.1, align 4, !tbaa !3
  %3 = load i32, i32* %arrayidx11.1, align 4, !tbaa !3
  %mul.1 = mul nsw i32 %3, %2
  %add.1 = add nsw i32 %mul, %mul.1
  store i32 %add.1, i32* %arrayidx4, align 4, !tbaa !3
  %arrayidx9.2 = getelementptr inbounds [4 x i32], [4 x i32]* %mat1, i32 %i.037, i32 2
  %4 = load i32, i32* %arrayidx9.2, align 4, !tbaa !3
  %5 = load i32, i32* %arrayidx11.2, align 4, !tbaa !3
  %mul.2 = mul nsw i32 %5, %4
  %add.2 = add nsw i32 %add.1, %mul.2
  store i32 %add.2, i32* %arrayidx4, align 4, !tbaa !3
  %arrayidx9.3 = getelementptr inbounds [4 x i32], [4 x i32]* %mat1, i32 %i.037, i32 3
  %6 = load i32, i32* %arrayidx9.3, align 4, !tbaa !3
  %7 = load i32, i32* %arrayidx11.3, align 4, !tbaa !3
  %mul.3 = mul nsw i32 %7, %6
  %add.3 = add nsw i32 %add.2, %mul.3
  store i32 %add.3, i32* %arrayidx4, align 4, !tbaa !3
  %arrayidx4.1 = getelementptr inbounds [4 x i32], [4 x i32]* %res, i32 %i.037, i32 1
  store i32 0, i32* %arrayidx4.1, align 4, !tbaa !3
  %8 = load i32, i32* %arrayidx9, align 4, !tbaa !3
  %9 = load i32, i32* %arrayidx11.1.39, align 4, !tbaa !3
  %mul.1.40 = mul nsw i32 %9, %8
  store i32 %mul.1.40, i32* %arrayidx4.1, align 4, !tbaa !3
  %10 = load i32, i32* %arrayidx9.1, align 4, !tbaa !3
  %11 = load i32, i32* %arrayidx11.1.1, align 4, !tbaa !3
  %mul.1.1 = mul nsw i32 %11, %10
  %add.1.1 = add nsw i32 %mul.1.40, %mul.1.1
  store i32 %add.1.1, i32* %arrayidx4.1, align 4, !tbaa !3
  %12 = load i32, i32* %arrayidx9.2, align 4, !tbaa !3
  %13 = load i32, i32* %arrayidx11.2.1, align 4, !tbaa !3
  %mul.2.1 = mul nsw i32 %13, %12
  %add.2.1 = add nsw i32 %add.1.1, %mul.2.1
  store i32 %add.2.1, i32* %arrayidx4.1, align 4, !tbaa !3
  %14 = load i32, i32* %arrayidx9.3, align 4, !tbaa !3
  %15 = load i32, i32* %arrayidx11.3.1, align 4, !tbaa !3
  %mul.3.1 = mul nsw i32 %15, %14
  %add.3.1 = add nsw i32 %add.2.1, %mul.3.1
  store i32 %add.3.1, i32* %arrayidx4.1, align 4, !tbaa !3
  %arrayidx4.2 = getelementptr inbounds [4 x i32], [4 x i32]* %res, i32 %i.037, i32 2
  store i32 0, i32* %arrayidx4.2, align 4, !tbaa !3
  %16 = load i32, i32* %arrayidx9, align 4, !tbaa !3
  %17 = load i32, i32* %arrayidx11.2.42, align 4, !tbaa !3
  %mul.2.43 = mul nsw i32 %17, %16
  store i32 %mul.2.43, i32* %arrayidx4.2, align 4, !tbaa !3
  %18 = load i32, i32* %arrayidx9.1, align 4, !tbaa !3
  %19 = load i32, i32* %arrayidx11.1.2, align 4, !tbaa !3
  %mul.1.2 = mul nsw i32 %19, %18
  %add.1.2 = add nsw i32 %mul.2.43, %mul.1.2
  store i32 %add.1.2, i32* %arrayidx4.2, align 4, !tbaa !3
  %20 = load i32, i32* %arrayidx9.2, align 4, !tbaa !3
  %21 = load i32, i32* %arrayidx11.2.2, align 4, !tbaa !3
  %mul.2.2 = mul nsw i32 %21, %20
  %add.2.2 = add nsw i32 %add.1.2, %mul.2.2
  store i32 %add.2.2, i32* %arrayidx4.2, align 4, !tbaa !3
  %22 = load i32, i32* %arrayidx9.3, align 4, !tbaa !3
  %23 = load i32, i32* %arrayidx11.3.2, align 4, !tbaa !3
  %mul.3.2 = mul nsw i32 %23, %22
  %add.3.2 = add nsw i32 %add.2.2, %mul.3.2
  store i32 %add.3.2, i32* %arrayidx4.2, align 4, !tbaa !3
  %arrayidx4.3 = getelementptr inbounds [4 x i32], [4 x i32]* %res, i32 %i.037, i32 3
  store i32 0, i32* %arrayidx4.3, align 4, !tbaa !3
  %24 = load i32, i32* %arrayidx9, align 4, !tbaa !3
  %25 = load i32, i32* %arrayidx11.3.45, align 4, !tbaa !3
  %mul.3.46 = mul nsw i32 %25, %24
  store i32 %mul.3.46, i32* %arrayidx4.3, align 4, !tbaa !3
  %26 = load i32, i32* %arrayidx9.1, align 4, !tbaa !3
  %27 = load i32, i32* %arrayidx11.1.3, align 4, !tbaa !3
  %mul.1.3 = mul nsw i32 %27, %26
  %add.1.3 = add nsw i32 %mul.3.46, %mul.1.3
  store i32 %add.1.3, i32* %arrayidx4.3, align 4, !tbaa !3
  %28 = load i32, i32* %arrayidx9.2, align 4, !tbaa !3
  %29 = load i32, i32* %arrayidx11.2.3, align 4, !tbaa !3
  %mul.2.3 = mul nsw i32 %29, %28
  %add.2.3 = add nsw i32 %add.1.3, %mul.2.3
  store i32 %add.2.3, i32* %arrayidx4.3, align 4, !tbaa !3
  %30 = load i32, i32* %arrayidx9.3, align 4, !tbaa !3
  %31 = load i32, i32* %arrayidx11.3.3, align 4, !tbaa !3
  %mul.3.3 = mul nsw i32 %31, %30
  %add.3.3 = add nsw i32 %add.2.3, %mul.3.3
  store i32 %add.3.3, i32* %arrayidx4.3, align 4, !tbaa !3
  %inc18 = add nuw nsw i32 %i.037, 1
  %exitcond = icmp eq i32 %inc18, 4
  br i1 %exitcond, label %for.end.19, label %for.cond.1.preheader

for.end.19:                                       ; preds = %for.cond.1.preheader
  ret void
}

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %mat1 = alloca [4 x [4 x i32]], align 4
  %mat2 = alloca [4 x [4 x i32]], align 4
  %res = alloca [4 x [4 x i32]], align 4
  %0 = bitcast [4 x [4 x i32]]* %mat1 to i8*
  call void @llvm.lifetime.start(i64 64, i8* %0) #1
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %0, i8* bitcast ([4 x [4 x i32]]* @main.mat2 to i8*), i32 64, i32 4, i1 false)
  %1 = bitcast [4 x [4 x i32]]* %mat2 to i8*
  call void @llvm.lifetime.start(i64 64, i8* %1) #1
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %1, i8* bitcast ([4 x [4 x i32]]* @main.mat2 to i8*), i32 64, i32 4, i1 false)
  %2 = bitcast [4 x [4 x i32]]* %res to i8*
  call void @llvm.lifetime.start(i64 64, i8* %2) #1
  %arraydecay = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %mat1, i32 0, i32 0
  %arraydecay1 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %mat2, i32 0, i32 0
  %arraydecay2 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 0
  call void @multiply([4 x i32]* %arraydecay, [4 x i32]* %arraydecay1, [4 x i32]* %arraydecay2)
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0))
  %arrayidx6 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 0, i32 0
  %3 = load i32, i32* %arrayidx6, align 4, !tbaa !3
  %call7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %3) #1
  %arrayidx6.1 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 0, i32 1
  %4 = load i32, i32* %arrayidx6.1, align 4, !tbaa !3
  %call7.1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %4) #1
  %arrayidx6.2 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 0, i32 2
  %5 = load i32, i32* %arrayidx6.2, align 4, !tbaa !3
  %call7.2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %5) #1
  %arrayidx6.3 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 0, i32 3
  %6 = load i32, i32* %arrayidx6.3, align 4, !tbaa !3
  %call7.3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %6) #1
  %putchar = tail call i32 @putchar(i32 10) #1
  %arrayidx6.1.20 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 1, i32 0
  %7 = load i32, i32* %arrayidx6.1.20, align 4, !tbaa !3
  %call7.1.21 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %7) #1
  %arrayidx6.1.1 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 1, i32 1
  %8 = load i32, i32* %arrayidx6.1.1, align 4, !tbaa !3
  %call7.1.1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %8) #1
  %arrayidx6.2.1 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 1, i32 2
  %9 = load i32, i32* %arrayidx6.2.1, align 4, !tbaa !3
  %call7.2.1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %9) #1
  %arrayidx6.3.1 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 1, i32 3
  %10 = load i32, i32* %arrayidx6.3.1, align 4, !tbaa !3
  %call7.3.1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %10) #1
  %putchar.1 = tail call i32 @putchar(i32 10) #1
  %arrayidx6.2.22 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 2, i32 0
  %11 = load i32, i32* %arrayidx6.2.22, align 4, !tbaa !3
  %call7.2.23 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %11) #1
  %arrayidx6.1.2 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 2, i32 1
  %12 = load i32, i32* %arrayidx6.1.2, align 4, !tbaa !3
  %call7.1.2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %12) #1
  %arrayidx6.2.2 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 2, i32 2
  %13 = load i32, i32* %arrayidx6.2.2, align 4, !tbaa !3
  %call7.2.2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %13) #1
  %arrayidx6.3.2 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 2, i32 3
  %14 = load i32, i32* %arrayidx6.3.2, align 4, !tbaa !3
  %call7.3.2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %14) #1
  %putchar.2 = tail call i32 @putchar(i32 10) #1
  %arrayidx6.3.24 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 3, i32 0
  %15 = load i32, i32* %arrayidx6.3.24, align 4, !tbaa !3
  %call7.3.25 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %15) #1
  %arrayidx6.1.3 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 3, i32 1
  %16 = load i32, i32* %arrayidx6.1.3, align 4, !tbaa !3
  %call7.1.3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %16) #1
  %arrayidx6.2.3 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 3, i32 2
  %17 = load i32, i32* %arrayidx6.2.3, align 4, !tbaa !3
  %call7.2.3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %17) #1
  %arrayidx6.3.3 = getelementptr inbounds [4 x [4 x i32]], [4 x [4 x i32]]* %res, i32 0, i32 3, i32 3
  %18 = load i32, i32* %arrayidx6.3.3, align 4, !tbaa !3
  %call7.3.3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %18) #1
  %putchar.3 = tail call i32 @putchar(i32 10) #1
  call void @llvm.lifetime.end(i64 64, i8* %2) #1
  call void @llvm.lifetime.end(i64 64, i8* %1) #1
  call void @llvm.lifetime.end(i64 64, i8* %0) #1
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture, i8* nocapture readonly, i32, i32, i1) #1

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #0

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) #1

; Function Attrs: nounwind
declare i32 @putchar(i32) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a53" "target-features"="+crc,+crypto,+fp-armv8,+hwdiv,+hwdiv-arm,+neon" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"min_enum_size", i32 4}
!2 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
