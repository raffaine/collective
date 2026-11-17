import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

// 30 unique eye-level walking scenes across 54m 00s (~1m 48s per vista, zero repetition)
let scenes: [SceneSpec] = [
    // Part I: Scenario Zero — Sovereign Software, Games & Waste-Heat Compute (00:00 - 18:00)
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_01.png", actLabel: "PART I // SCENARIO ZERO: SOVEREIGN FORGE", placeLabel: "20-Year Salvaged ThinkPad & RISC-V Studio"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_04.png", actLabel: "PART I // SCENARIO ZERO: CAPABILITY NUTRITION", placeLabel: "Timber Studio & Watershed Contour Map"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_02.png", actLabel: "PART I // SCENARIO ZERO: THERMODYNAMIC COMPUTE", placeLabel: "Hydronic Server Heat-Recovery to Greenhouse"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_01.png", actLabel: "PART I // SCENARIO ZERO: N=1 HOMELAB SEED", placeLabel: "Solo Homesteader's Dusk Garage Workshop"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_03.png", actLabel: "PART I // SCENARIO ZERO: 80% HEAT CAPTURE", placeLabel: "Server-Warmed Cedar Bathhouse Soaking Pool"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_02.png", actLabel: "PART I // SCENARIO ZERO: DETOX QUARANTINE", placeLabel: "Reclaimed Brick Warehouse & Greenhouse Lane"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_04.png", actLabel: "PART I // SCENARIO ZERO: OASIS & GAME COMMONS", placeLabel: "Game Art, Luthiery & Modding Alcove"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_05.png", actLabel: "PART I // SCENARIO ZERO: 10,000-TICK CANARY", placeLabel: "Canopy Boardwalk Over Living Valley"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_05.png", actLabel: "PART I // SCENARIO ZERO: OFFLINE MESH SYNC", placeLabel: "Dusk Footpath & Community Radio Antenna"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_11.png", actLabel: "PART I // SCENARIO ZERO: COOPERATIVE STUDIO", placeLabel: "Street-Corner Third-Place Cafe & Library"),

    // Part II: Scenario Tau — Steward Genesis & Four-Pathway Inclusion (18:00 - 36:00)
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_06.png", actLabel: "PART II // SCENARIO TAU: 4-PATHWAY GENESIS", placeLabel: "Zero-Step Cedar Ramp & Sensory-Calm Slate"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_06.png", actLabel: "PART II // SCENARIO TAU: PATHWAY 1 NEIGHBOR", placeLabel: "Suburban Permaculture Street"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_07.png", actLabel: "PART II // SCENARIO TAU: 3-OF-5 SHAMIR RECOVERY", placeLabel: "Welcoming Tea Parlor & Five Stoneware Cups"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_02.png", actLabel: "PART II // SCENARIO TAU: 10,000 BP BASELINE", placeLabel: "Neighborhood Tool-Crib & Tiffin Porch"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_08.png", actLabel: "PART II // SCENARIO TAU: PATHWAY 4 WALK-IN", placeLabel: "Sanctuary Lodge & Physical Brass Room Key"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_09.png", actLabel: "PART II // SCENARIO TAU: UNIVERSAL INCLUSION", placeLabel: "Zero-Step Courtyard & Lavender Path"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_09.png", actLabel: "PART II // SCENARIO TAU: DISABILITY JUSTICE", placeLabel: "Accessible Sensory Herb & Orchard Path"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_07.png", actLabel: "PART II // SCENARIO TAU: ZERO BUY-IN PARITY", placeLabel: "Unfenced Backyard Garden Commons"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_10.png", actLabel: "PART II // SCENARIO TAU: R0 ASSISTIVE PERMANENCE", placeLabel: "Neighborhood Mobility & Assistive-Tech Shop"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_03.png", actLabel: "PART II // SCENARIO TAU: EQUAL COMMONS VOICE", placeLabel: "Circular Timber Council Pavilion & Meadow"),

    // Part III: Scenario Sigma — Burnout Homeostasis & Drudgery Automation (36:00 - 54:00)
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_11.png", actLabel: "PART III // SCENARIO SIGMA: ASYMMETRIC REST", placeLabel: "Wooden-and-Brass Bedroom Sanctuary Latch"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_09.png", actLabel: "PART III // SCENARIO SIGMA: <=300 CB QUIET", placeLabel: "Screenless Mass-Timber Living Room & Hearth"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_12.png", actLabel: "PART III // SCENARIO SIGMA: CARE LEAVE SHIELD", placeLabel: "Screenless Sunlit Conservatory & Reading Nook"),
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_10.png", actLabel: "PART III // SCENARIO SIGMA: SCHMITT HOMEOSTAT", placeLabel: "Suburban Greywater Reed-Bed Boardwalk"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_13.png", actLabel: "PART III // SCENARIO SIGMA: AUTOMATION QUESTS", placeLabel: "Rotary Barrel Root-Washer Harvest Station"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_07.png", actLabel: "PART III // SCENARIO SIGMA: 40% CIRCUIT BREAKER", placeLabel: "Sunlit Woodshop & Apprentice Pairing Bay"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_14.png", actLabel: "PART III // SCENARIO SIGMA: ROTATIONAL WORK-BEES", placeLabel: "Post-Work-Bee Courtyard Tea & Sourdough Table"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_10.png", actLabel: "PART III // SCENARIO SIGMA: UNMEDIATED JOY", placeLabel: "Acoustic Music Parlor & Warm Lanterns"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_15.png", actLabel: "PART III // SCENARIO SIGMA: SOMATIC RECOVERY", placeLabel: "Twilight Stream Path to Cedar Sauna"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_15.png", actLabel: "PART III // SCENARIO SIGMA: CALM COMMONWEALTH", placeLabel: "Reflecting Pond Footbridge & Twilight Hills")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 5.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 700, height: 86)
    ctx.setFillColor(CGColor(red: 0.07, green: 0.08, blue: 0.10, alpha: 0.62))
    let path = CGPath(roundedRect: boxRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.addPath(path)
    ctx.fillPath()

    ctx.setFillColor(CGColor(red: 0.95, green: 0.77, blue: 0.19, alpha: 0.90))
    ctx.fill(CGRect(x: 48, y: 42, width: 4, height: 86))

    let graphicsCtx = NSGraphicsContext(cgContext: ctx, flipped: false)
    NSGraphicsContext.saveGraphicsState()
    NSGraphicsContext.current = graphicsCtx

    let topAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.monospacedSystemFont(ofSize: 13, weight: .bold),
        .foregroundColor: NSColor(calibratedRed: 0.95, green: 0.77, blue: 0.19, alpha: 0.92)
    ]
    let midAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.systemFont(ofSize: 18, weight: .semibold),
        .foregroundColor: NSColor(calibratedWhite: 0.96, alpha: 0.95)
    ]
    let botAttrs: [NSAttributedString.Key: Any] = [
        .font: NSFont.monospacedSystemFont(ofSize: 12, weight: .regular),
        .foregroundColor: NSColor(calibratedWhite: 0.78, alpha: 0.85)
    ]

    NSString(string: "RADIO LAMBDA // EP. 04: WAVE A (ZERO, TAU, SIGMA)").draw(at: NSPoint(x: 66, y: 99), withAttributes: topAttrs)
    NSString(string: placeLabel).draw(at: NSPoint(x: 66, y: 71), withAttributes: midAttrs)
    NSString(string: actLabel).draw(at: NSPoint(x: 66, y: 50), withAttributes: botAttrs)

    NSGraphicsContext.restoreGraphicsState()
    return ctx.makeImage()!
}

func drawWalkingScene(into ctx: CGContext, img: CGImage, progress: Double, index: Int, alpha: CGFloat = 1.0) {
    let smooth = progress * progress * (3.0 - 2.0 * progress)
    let zoom = CGFloat(1.0 + 0.12 * smooth)
    let panDir: CGFloat = (index % 2 == 0) ? 1.0 : -1.0
    let panX = panDir * CGFloat(18.0 * (smooth - 0.5))
    let panY = CGFloat(-8.0 * (smooth - 0.5))

    let drawW = CGFloat(width) * zoom
    let drawH = CGFloat(height) * zoom
    let dx = (CGFloat(width) - drawW) * 0.5 + panX
    let dy = (CGFloat(height) - drawH) * 0.48 + panY

    if alpha < 1.0 {
        ctx.saveGState()
        ctx.setAlpha(alpha)
        ctx.draw(img, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
        ctx.restoreGState()
    } else {
        ctx.draw(img, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    }
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    let audioPath = "doc/podcast/episodes/04.episode.m4a"
    let tempVideoPath = "/tmp/ep04_silent_video.mp4"
    let finalOutputPath = "doc/podcast/episodes/04.episode_youtube.mp4"

    try? FileManager.default.removeItem(atPath: tempVideoPath)
    try? FileManager.default.removeItem(atPath: finalOutputPath)

    let audioAsset = AVURLAsset(url: URL(fileURLWithPath: audioPath))
    let durationCM = try await audioAsset.load(.duration)
    let totalDurationSec = CMTimeGetSeconds(durationCM)
    print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", totalDurationSec, totalDurationSec / 60.0))

    let baseImages = scenes.map { loadNormalizedCGImage($0.path) }
    let hudImages = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }

    let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
    let videoSettings: [String: Any] = [
        AVVideoCodecKey: AVVideoCodecType.h264,
        AVVideoWidthKey: width,
        AVVideoHeightKey: height,
        AVVideoCompressionPropertiesKey: [
            AVVideoAverageBitRateKey: 1_800_000,
            AVVideoMaxKeyFrameIntervalKey: Int(fps) * 3
        ]
    ]
    let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
    writerInput.expectsMediaDataInRealTime = false

    let pixelBufferAttrs: [String: Any] = [
        kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
        kCVPixelBufferWidthKey as String: width,
        kCVPixelBufferHeightKey as String: height
    ]
    let adaptor = AVAssetWriterInputPixelBufferAdaptor(
        assetWriterInput: writerInput,
        sourcePixelBufferAttributes: pixelBufferAttrs
    )

    writer.add(writerInput)
    writer.startWriting()
    writer.startSession(atSourceTime: .zero)

    let totalFrames = Int(ceil(totalDurationSec * Double(fps)))
    let segDuration = totalDurationSec / Double(scenes.count)

    print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

    for frameIdx in 0..<totalFrames {
        while !writerInput.isReadyForMoreMediaData {
            usleep(2000)
        }
        autoreleasepool {
            let t = Double(frameIdx) / Double(fps)
            let rawIdx = Int(t / segDuration)
            let idx = min(max(rawIdx, 0), scenes.count - 1)
            let segStart = Double(idx) * segDuration
            let localT = t - segStart
            let progress = min(max(localT / segDuration, 0.0), 1.0)

            var pb: CVPixelBuffer?
            CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pb)
            if let pixelBuffer = pb {
                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                if let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer),
                   let ctx = CGContext(
                       data: baseAddr,
                       width: width,
                       height: height,
                       bitsPerComponent: 8,
                       bytesPerRow: CVPixelBufferGetBytesPerRow(pixelBuffer),
                       space: colorSpace,
                       bitmapInfo: bitmapInfo
                   ) {
                    ctx.interpolationQuality = .medium
                    drawWalkingScene(into: ctx, img: baseImages[idx], progress: progress, index: idx)

                    var activeHud = hudImages[idx]
                    if idx < scenes.count - 1 && (segDuration - localT) < crossfadeSec {
                        let fadeAlpha = CGFloat(1.0 - (segDuration - localT) / crossfadeSec)
                        let nextProgress = max(0.0, (localT - (segDuration - crossfadeSec)) / segDuration)
                        drawWalkingScene(into: ctx, img: baseImages[idx + 1], progress: nextProgress, index: idx + 1, alpha: fadeAlpha)
                        if fadeAlpha > 0.5 {
                            activeHud = hudImages[idx + 1]
                        }
                    }
                    ctx.draw(activeHud, in: CGRect(x: 0, y: 0, width: width, height: height))
                }
                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let frameTime = CMTime(value: Int64(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: frameTime)
            }
        }

        if frameIdx % (Int(fps) * 240) == 0 && frameIdx > 0 {
            let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
            print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
        }
    }

    writerInput.markAsFinished()
    await writer.finishWriting()
    print("Silent video track rendered. Now muxing audio losslessly...")

    let mixComp = AVMutableComposition()
    let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

    guard let compVideoTrack = mixComp.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid),
          let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first else {
        fatalError("Failed to load video track for muxing")
    }
    try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

    guard let compAudioTrack = mixComp.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid),
          let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first else {
        fatalError("Failed to load audio track for muxing")
    }
    try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

    guard let exporter = AVAssetExportSession(asset: mixComp, presetName: AVAssetExportPresetPassthrough) else {
        fatalError("Failed to create AVAssetExportSession")
    }
    try await exporter.export(to: URL(fileURLWithPath: finalOutputPath), as: .mp4)
    try? FileManager.default.removeItem(atPath: tempVideoPath)
    print("SUCCESS: Created YouTube video at \(finalOutputPath)")
    semaphore.signal()
}

semaphore.wait()
